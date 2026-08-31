# in Analysis/bin run "bash lxplusCopy.sh"
#scp DarkPhotonSignal.html chenm@lxplus.cern.ch:/eos/user/c/chenm/www
#scp jumboPlotStorage/DarkPhotonSignal/*.png chenm@lxplus.cern.ch:/eos/user/c/chenm/www/jumboPlotStorage/DarkPhotonSignal

#scp HiggsSignal.html chenm@lxplus.cern.ch:/eos/user/c/chenm/www
# in Analysis/bin run "bash lxplusCopy.sh"

scp HiggsSignal4ChannelZPeak.html \
    HiggsSignal3ChannelZPeak.html \
    HiggsSignal2ChannelZPeak.html \
    HiggsSignal4ChannelNonZPeak.html \
    HiggsSignal3ChannelNonZPeak.html \
    HiggsSignal2ChannelNonZPeak.html \
	DarkPhotonsSignalNonZPeak.html \
	DarkPhotonsControlNonZPeak.html \
    index.html \
    style.css \
    ncatlett@lxplus.cern.ch:/eos/user/n/ncatlett/www/

scp jumboPlotStorage/HiggsSignal/*.png ncatlett@lxplus.cern.ch:/eos/user/n/ncatlett/www/jumboPlotStorage
scp jumboPlotStorage/DarkPhoton/*.png ncatlett@lxplus.cern.ch:/eos/user/n/ncatlett/www/jumboPlotStorage