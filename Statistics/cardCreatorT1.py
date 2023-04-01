import os

#def main():
#stream = os.popen('combine -M BayesianToyMC datacard_part1.txt -t 2 --noDefaultPrior=0')
#output = stream.read()
#print(output)
#outputArray = output.split("mean   expected limit: r <")
#rawResults = outputArray[1]
#print(rawResults)



#rawResults = rawResults.split("/n")



def AddBin(WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1, Signal):
    BinLine = BinLine + "Lep" + line1[b+1]
    BinLine = BinLine + (" " * (10-len(line1[b+1])))
    Pname = Signal[0]
    if len(Pname) > 12:
        Pname = Pname[0:12]
    ProcessLine1 = ProcessLine1 + Pname
    ProcessLine1 = ProcessLine1 + (" " * (13 - len(Pname)))
    ProcessLine2 = ProcessLine2 + "0"
    ProcessLine2 = ProcessLine2 + (" " * (12))
    CurrentRate = str(Signal[b+1])
    if len(CurrentRate) > 12:
        CurrentRate = CurrentRate[0:12]
    RateLine = RateLine + CurrentRate
    RateLine = RateLine + (" " * (13 - len(CurrentRate)))

    for i in range(0,Pcount):
        BinLine = BinLine + "Lep" + line1[b+1]
        BinLine = BinLine + (" " * (10-len(line1[b+1])))
        ProcessLine1 = ProcessLine1 + Plist[WhichP[i] -1 ][0]
        ProcessLine1 = ProcessLine1 + (" " * (13 - len(Plist[WhichP[i] -1 ][0])))
        ProcessLine2 = ProcessLine2 + str(i+1)
        ProcessLine2 = ProcessLine2 + (" " * (13 - len(str(i+1))))
        CurrentRate = str(Plist[WhichP[i] -1 ][b +1 ])
        if len(CurrentRate) > 12:
            CurrentRate = CurrentRate[0:12]
        RateLine = RateLine + CurrentRate
        RateLine = RateLine + (" " * (13 - len(CurrentRate)))
    return (WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1)
        



##a= Iparameters.split("\n")
##f.writelines(a)
WhichP = [1,2,4,5,7]

NoUC = input('# of uncertanties: ')
Title = input('Insert Datacard Title: ')
##f = open(Title, "w")
Info = open(Title, "r")
Parameters = Info.read()
lines = Parameters.split("\n")
print(lines)

line1 = lines[0].split("\t")
Signal = lines[1].split("\t")
##print(Signal)
Plist = []
for i in range (3,10):
    CurrentLine = lines[i].split("\t")
    Plist.append(CurrentLine)

Bcount = (len(line1)-1)
Pcount = len(WhichP)


#Create the top part
Text = "imax "
Text = Text + str(Bcount) + " number of bins\n"
Text = Text + "jmax 5 number of processes minus 1\n"
Text = Text + "kmax " + NoUC + " number of nuisance parameters\n"
Text = Text + "----------------------------------------------------------------------------------------------------------------------------------\n"
Currentline = "bin          "
for i in range (1,len(line1)):
    Currentline = Currentline + "Lep" + line1[i] + "        "
Text = Text + Currentline + "\n"
Text = Text + "observation  " + ("43.420453  " * Bcount) +"\n"
Text = Text + "----------------------------------------------------------------------------------------------------------------------------------\n"
BinLine = "bin          "
ProcessLine1 = "process      "
ProcessLine2 = "process      "
RateLine = "rate         "

for b in range (0,Bcount):
    (WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1) = AddBin(WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1, Signal)
Text = Text + BinLine + "\n"
Text = Text + ProcessLine1 + "\n"
Text = Text + ProcessLine2 + "\n"
Text = Text + RateLine + "\n"
Text = Text + "----------------------------------------------------------------------------------------------------------------------------------\n"


if NoUC != 0:
    #add uncertainties
    UCtext = ""#Formated string of just the uncertainties


    UC = []#UC stands for uncertainties

    print("Input each part of the uncertainties reading left to right while including names but excluding LnN and spaces")
    for i in range (0,int(NoUC)):
        Templist = []
        for e in range (0,Bcount * Pcount + 1):
            x = input()
            Templist.append(x)
        UC.append(Templist)



    #format list to datacard

    Method = "lnN     "#Choose which method is used

    for i in range (0,int(NoUC)):
        UCtext = UCtext + UC[i][0] + (" " * (19 - len(UC[i][0])))#add name of uncertainty
        UCtext = UCtext + Method
        for e in range (1,Bcount * Pcount + 1):
            CUC = UC[i][e]
            CUC = CUC + (" " * (16 - len(CUC)))
            UCtext = UCtext + CUC
        UCtext = UCtext + "\n"

    Text = Text + UCtext


Info.close()
##print(Plist)
##print(Plist[0][1])
#f = open(Title, "w")
#f.write(Text)
print(Text)
#f.close()





##ParameterSignal = Info.readline()
##Info.readline()#Go past blank space
##for i in range (2,Pcount - 1):
##    print(Info.readline(i))
##f.close()
##Info.close()



##Leptons	2	3	4	5+
##H++ Signal 4L	6.23761	58.0743	115.5750267	2.007506667
##				
##ZZ	13.44192016	12.71203437	4.709051371	0.515213498
##WZ3LNu	102.666411	17.7567692	1.614251745	1.937102095
##TTBar2	627123.3239	730736.7689	214672.8287	31426.33867
##DY Electrons	953.5070572	344.6879012	0.0233859	0
##DY Muons	8325.101091	1711.652696	136.2913774	28.27418185
##DY Jets				
##TTBar	5791.675	2133.775	304.825	0
##YOu can only read a document once
