void walk(TDirectory* d, TString path)
{
    TIter next(d->GetListOfKeys());
    TKey* k;
    while ((k = (TKey*)next()))
    {
        TObject* o = k->ReadObj();
        TString p = path + "/" + k->GetName();
        if (o->InheritsFrom("TDirectory"))
        {
            walk((TDirectory*)o, p);
        }
        else if (o->InheritsFrom("TH1"))
        {
            TH1* h = (TH1*)o;
            if (p.Contains("eeet"))
            {
                printf("%s | entries=%.0f mean=%.4f rms=%.4f\n", p.Data(), h->GetEntries(), h->GetMean(), h->GetRMS());
            }
        }
    }
}

void walk_hists(const char* fname)
{
    TFile* f = TFile::Open(fname);
    walk(f, "");
}
