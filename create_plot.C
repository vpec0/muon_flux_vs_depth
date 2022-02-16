

#define FOR(i, size) for (int i = 0; i < size; ++i)

void DrawText(const char* text, double x, double y, int left = 1, float vertical_offset = 0., float horizontal_offset = 0.);

struct USite_t {
    const char* name;
    int isFlat;
    double depth;
    double flux;
    double fluxErr;
};


enum {
    kMountain = 0,
    kFlat = 1
};

// data
std::vector<USite_t> sites = {
    // S.Kamat,Extending the sensitivity..., Ph.D. thesis, Case Western Reserve University (2005).
    // {"Soudan"	,	kFlat,		2100.,	2.0e-7,		0.2e-7},
    // C. Zhang and D.-M. Mei, Phys. Rev. D 90, 122003 (2014)
    {"Soudan"		,	kFlat,		2100.,	1.65e-7,	0.1e-7},
    // Canfranc, arXiv:1902.00868 [physics.ins-det]; Depth: https://doi.org/10.1142/9789812701848_0067
    {"LSC"		,	kMountain,	2450.,	5.26e-7,	0.21e-7},
    // {"Canfranc LAB2400"	,	kMountain,	2450.,	5.26e-7,	0.21e-7},
    // {"Canfranc LAB2500"	,	kMountain,	2450.,	4.29e-7,	0.17e-7},
    {"Kamioka"		,	kMountain,	2700.,	1.58e-7,	0.21e-7},
    // M. Robinson, et al., Nucl. Instrum. Methods A 511 (3) (2003) 347-353.
    // {"Boulby"	,	kFlat,		2805.,	4.09e-8,	0.15e-8},
    // L.Reichhart et al. Astropart. Phys. 47 (2013) 67-76
    {"Boulby 1100"	,	kFlat,		2850.,	3.75e-8,	0.09e-8},
    // Gran Sasso, M. Agostini et al JCAP02(2019)046
    {"LNGS"		,	kMountain,	3800.,	3.432e-8,	0.003e-8},
    // Frejus
    // Phys. Rev. D 40 (1989) 2163-2171
    {"LSM"		,	kMountain,	4850.,	5.47e-9,	0.1e-9},
    // Phys. Rev. D 80 (2009) 012001
    {"SNOLAB"		,	kFlat,		5890.,	3.31e-10,	0.01e-10},
    // Chinese Phys. C 45 (2021) 025001
    {"CJPL"		,	kMountain,	6720.,	3.53e-10,	0.22e-10},
    // Astroparticle Physics 93 (2017) 70-75
    {"SURF"             ,       kFlat,          4200.,  5.31e-9,        0.17e-9},
    // "Pyh#ddot{a}salmi"
    // Enquist et al., Nucl. Instrum. Methods A 554 (1-3) 286 (2005)
    {"CallioLab 400"	,       kFlat,           980.,  2.1e-6,         0.2e-6},
    {"CallioLab 660"	,       kFlat,          1900.,  3.2e-7,         0.3e-7},
    {"CallioLab 990"	,       kFlat,          2810.,  6.2e-8,         0.6e-8},
    { "CallioLab 1390"	,       kFlat,          3960.,  1.1e-8,         0.1e-8},
};

const int NEXPS = sites.size();

USite_t Boulby1400 = {"Boulby 1400", kFlat, 3575, 1.13e-8, 0.}; //9.85e-9, 0.};

const int colors[2] = {kBlue, kBlack};
const int markers[2] = {kFullSquare, kFullCircle};


// Calculated Intensity-depth relation for standard rock, flat overburden, MUSIC paper
// arXiv:0810.4635 [physics.comp-ph]; 10.1016/j.cpc.2008.10.013
double prediction[11][2] = {
    { 0.5, 2.07E-05},
    { 1.0, 2.56E-06},
    { 2.0, 1.99E-07},
    { 3.0, 3.09E-08},
    { 4.0, 6.38E-09},
    { 5.0, 1.53E-09},
    { 6.0, 4.02E-10},
    { 7.0, 1.12E-10},
    { 8.0, 3.23E-11},
    { 9.0, 9.61E-12},
    { 10.0, 2.91E-12},
};

void create_plot()
{
    gStyle->SetPadGridX(1);
    gStyle->SetPadGridY(1);

    gStyle->SetMarkerSize(1.);

    gStyle->SetTitleOffset(1.2, "x");
    gStyle->SetTitleOffset(1.2, "y");

    // canvas margins
    const float margin = 0.15;
    gStyle->SetPadLeftMargin  (margin);
    gStyle->SetPadRightMargin (margin);
    gStyle->SetPadBottomMargin(margin);
    gStyle->SetPadTopMargin   (margin);

    // axis labels and titles
    gStyle->SetLabelFont(43, "xy");
    gStyle->SetLabelSize(18, "xy");

    gStyle->SetTitleFont(43, "xy");
    gStyle->SetTitleSize(22, "xy");


    auto gr_flat = new TGraphErrors(NEXPS);
    auto gr_notflat = new TGraphErrors(NEXPS);
    gr_flat->SetMarkerColor(colors[1]);
    gr_flat->SetMarkerStyle(markers[1]);
    gr_notflat->SetMarkerColor(colors[0]);
    gr_notflat->SetMarkerStyle(markers[0]);

    int n_flat = 0;
    int n_notflat = 0;
    FOR(i, NEXPS) {
	if (sites[i].isFlat) {
	    gr_flat->SetPoint(n_flat, sites[i].depth, sites[i].flux);
	    gr_flat->SetPointError(n_flat, 0, sites[i].fluxErr);
	    n_flat++;
	} else {
	    gr_notflat->SetPoint(n_notflat, sites[i].depth, sites[i].flux);
	    gr_notflat->SetPointError(n_notflat, 0, sites[i].fluxErr);
	    n_notflat++;
	}
    }

    gr_flat->Set(n_flat);
    gr_notflat->Set(n_notflat);

    auto c = new TCanvas("c","");
    //c->DrawFrame(1e3, 1e-10, 7.5e3, 1e-6);




    auto mg = new TMultiGraph();
    mg->Add(gr_flat);
    mg->Add(gr_notflat);


    // Add Boulby 1400
    auto gr = new TGraphErrors(1);
    gr->SetPoint(0, Boulby1400.depth, Boulby1400.flux);
    gr->SetMarkerColor(kRed);
    gr->SetMarkerStyle(kOpenDiamond);
    gr->SetMarkerSize(1.5);
    mg->Add(gr);

    mg->SetTitle(";Vertical depth [m w.e.];Muon flux [cm^{-2} s^{-1}]");
    mg->Draw("AP");
    gPad->SetLogy();
    mg->SetMaximum(5e-6);
    mg->SetMinimum(1e-10);
    mg->GetXaxis()->SetLimits(0., 8.4e3);
    //gPad->Update();

    // centre axis titles
    // mg->GetXaxis()->CenterTitle();
    // mg->GetYaxis()->CenterTitle();

    // auto ax = (TGaxis*) gr->GetYaxis();
    // ax->SetOption("W");

    FOR(i, NEXPS) {
	// if (i == 1) // Canfranc LAB2400
	//     DrawText(sites[i].name, sites[i].depth, sites[i].flux * 1.2, sites[i].isFlat);
	// else if (i == 2) // Canfranc LAB2500
	//     DrawText(sites[i].name, sites[i].depth, sites[i].flux * 0.8, sites[i].isFlat);
	if ( strstr(sites[i].name, "CallioLab") )
	    if (!strstr(sites[i].name, "660") ) // Pyhasalmi
		DrawText(sites[i].name, sites[i].depth, sites[i].flux, 0);
	    else
		DrawText(sites[i].name, sites[i].depth, sites[i].flux, 1, 0., 100.);
	else
	    DrawText(sites[i].name, sites[i].depth, sites[i].flux, sites[i].isFlat);
    }

    // Boulby 1400
    DrawText(Boulby1400.name, Boulby1400.depth, Boulby1400.flux, Boulby1400.isFlat);


    // legend
    float width = 0.16;
    float height = 0.16;
    float leg_margin = 1. - margin - 0.03;
    auto leg = new TLegend(leg_margin-width, leg_margin-height, leg_margin, leg_margin);
    leg->SetTextFont(43);
    leg->SetTextSizePixels(18);
    leg->SetMargin(0.3);
    leg->SetHeader("Overburden", "c");
    leg->SetBorderSize(1);
    leg->AddEntry(gr_flat, "Flat", "p");
    leg->AddEntry(gr_notflat, "Mountain", "p");
    // make header bold font
    auto header = (TLegendEntry*)leg->GetListOfPrimitives()->First();
    header->SetTextFont(63);
    leg->Draw();


    c->SaveAs("total_mu_flux_vs_vertical_depth.pdf");


    // calculated prediction
    auto gr_pred = new TGraph(11);
    FOR(i, 11) {
	gr_pred->SetPoint(i, prediction[i][0]*1e3, prediction[i][1]);
    }
    gr_pred->Print();
    gr_pred->SetLineColor(kRed);
    //gr_pred->Draw("C");
    gr_pred->Draw("L");


    c->SaveAs("total_mu_flux_vs_vertical_depth_w_prediction.pdf");


    // version without grid lines
    c->SetGridx(0);
    c->SetGridy(0);
    leg->SetBorderSize(0);
    c->SaveAs("total_mu_flux_vs_vertical_depth_w_prediction_nogrid.pdf");

}


void DrawText(const char* text, double x, double y, int left, float vertical_offset, float horizontal_offset)
{
    auto tt = new TLatex(x + (1 - 2*left)*200 + horizontal_offset,
			 y + vertical_offset,
			 text);
    if (left) {
	tt->SetTextAlign(33);
    } else {
	tt->SetTextAlign(11);
    }
    tt->SetTextFont(43);
    tt->SetTextSize(18);
    tt->Draw();
}
