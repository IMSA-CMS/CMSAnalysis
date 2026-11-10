import json
import math
import ROOT

ROOT.gSystem.Load("libCMSAnalysisAnalysis")
ROOT.gInterpreter.Declare('''
#include "CMSAnalysis/Analysis/interface/DSCBFitFunction.hh"
#include "TBufferFile.h"
#include <memory>
double testDSCBValue(double x, const std::vector<double>& parameters) {
    DSCBFitFunction f("domain_test", -1e6, 1e6);
    return f.evaluateWithParameters(x, parameters);
}
bool testDSCBSerialization() {
    DSCBFitFunction f("template_io_test", 50, 2000);
    if (f.getFunction()->Eval(500) != 0) return false;
    TBufferFile output(TBuffer::kWrite);
    output.WriteObjectAny(&f, DSCBFitFunction::Class());
    TBufferFile input(TBuffer::kRead, output.Length(), output.Buffer(), false);
    std::unique_ptr<DSCBFitFunction> restored(static_cast<DSCBFitFunction*>(input.ReadObjectAny(DSCBFitFunction::Class())));
    const std::vector<double> p{1.9, 0.86, 1.5, 48, 500, 15, 40};
    return restored && restored->evaluateWithParameters(500, p) == f.evaluateWithParameters(500, p);
}
''')


def evaluate(x, p):
    return ROOT.testDSCBValue(x, p)


def original(x, p):
    a, b, n, m, mu, sigma, norm = p
    t = (x - mu) / sigma
    shape = math.exp(-t*t/2)
    if t < -a:
        factor = math.exp(-a*(-a-t)) if n > 1e12 else (1 + a*(-a-t)/n)**(-n)
        shape = math.exp(-a*a/2) * factor
    elif t > b:
        factor = math.exp(-b*(t-b)) if m > 1e12 else (1 + b*(t-b)/m)**(-m)
        shape = math.exp(-b*b/2) * factor
    z = math.sqrt(math.pi/2)*(math.erf(a/math.sqrt(2)) + math.erf(b/math.sqrt(2)))
    z += n/(n-1)/a*math.exp(-a*a/2) + m/(m-1)/b*math.exp(-b*b/2)
    return norm/sigma/z*shape


assert ROOT.testDSCBSerialization()
checks = 1
max_normalization_error = 0
for n in [-1e300, -2, 0.5, 1, math.nextafter(1, 2), 1.000001, 1.01, 1.5, 3, 10, 48.04581161419836, 1e3, 1e300]:
    for m in [0.5, 1, 1.000001, 2.7, 10, 1e300]:
        p = [1.9, 0.86, n, m, 500, 15, 40]
        bounded = p.copy()
        bounded[2:4] = [max(1.000001, v) for v in p[2:4]]
        for t in [-1e6, -20, -1.9, -0.1, 0, 0.86, 20, 1e6]:
            x = 500 + 15*t
            actual = evaluate(x, p)
            assert math.isfinite(actual) and actual >= 0, (p, t, actual)
            assert math.isclose(actual, original(x, bounded), rel_tol=2e-12, abs_tol=1e-300), (p, t, actual)
            checks += 1
        a, b, nl, nh, mu, sigma, norm = bounded
        steps = 1024
        dt = (a+b)/steps
        core = sum((1 if i in (0, steps) else 4 if i % 2 else 2) *
                   evaluate(mu + sigma*(-a+i*dt), p) for i in range(steps+1))*dt*sigma/3
        left = evaluate(mu-sigma*a, p)*sigma*(nl/(nl-1))/a
        right = evaluate(mu+sigma*b, p)*sigma*(nh/(nh-1))/b
        error = abs((core+left+right)/norm-1)
        max_normalization_error = max(max_normalization_error, error)
        assert error < 2e-11, (p, error)
        checks += 1
for index, values in [(0, [0, -1]), (1, [0, -1]), (5, [0, -1]), (6, [-1])]:
    for value in values:
        p = [1.9, 0.86, 1.5, 3, 500, 15, 40]
        p[index] = value
        try:
            evaluate(500, p)
        except Exception:
            checks += 1
        else:
            raise AssertionError(p)
for index in range(7):
    for value in [float('nan'), float('inf'), -float('inf')]:
        p = [1.9, 0.86, 1.5, 3, 500, 15, 40]
        p[index] = value
        try:
            evaluate(500, p)
        except Exception:
            checks += 1
        else:
            raise AssertionError(p)
for value in [float('nan'), float('inf'), -float('inf')]:
    try:
        evaluate(value, [1.9, 0.86, 1.5, 3, 500, 15, 40])
    except Exception:
        checks += 1
    else:
        raise AssertionError(value)
print(json.dumps({'checks': checks, 'lower_bound': 1.000001, 'upper_bound': None, 'max_full_line_normalization_error': max_normalization_error}))
