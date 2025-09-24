from os import environ
from os.path import dirname,sep,join,exists
cmssw_package_name=join(*__file__.split(sep)[-3:-1])
cmssw_base_dir=dirname(__file__.rsplit("/"+cmssw_package_name+"/",1)[0])
__path__.pop()
xdir=join(cmssw_base_dir,'src',cmssw_package_name, 'python')
if exists(xdir): __path__.append(xdir)
pyinit = join(xdir, "__init__.py")
if exists (pyinit):
  exec(open(pyinit).read())
else:
  __path__.append(join(cmssw_base_dir,'cfipython',environ['SCRAM_ARCH'],cmssw_package_name))
