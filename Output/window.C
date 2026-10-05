void window(const char* fname)
{
    TFile* f = TFile::Open(fname);
    const char* base = "hists/ZVeto/Reco eeet/HighMass/";
    const char* names[2] = {"Reco Same Sign Invariant Mass", "Corrected Reco Same Sign Invariant Mass"};
    for (int i = 0; i < 2; i++)
    {
        TH1* h = (TH1*)f->Get(TString(base) + names[i]);
        if (!h)
        {
            printf("MISSING %s\n", names[i]);
            continue;
        }
        int lo = h->GetXaxis()->FindBin(950.001);
        int hi = h->GetXaxis()->FindBin(1049.999);
        printf("%s | %s | total=%.0f in_950_1050=%.0f binwidth=%.2f\n", fname, names[i], h->Integral(0, h->GetNbinsX() + 1), h->Integral(lo, hi), h->GetBinWidth(1));
    }
}
