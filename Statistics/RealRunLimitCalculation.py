import os


#def valueFinder():

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
    print("----------------")
    print(b)
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



def makeCards(cardName, infoName,NofChannels, WhichP, NoUC, Systematics):
    #WhichP = [1,2,3,4] #1,2,4,5,7
    
    #NoUC = input('How many uncertainties do you have?')
    #NoUC = "0"
    Pcount = len(WhichP)
    ###print("Insert Datacard Title")
    #Title = input()
    Title = "cardName"
    ##f = open(Title, "w")
    Info = open(infoName, "r")
    Parameters = Info.read()
    lines = Parameters.split("\n")
    ###print(lines)

    line1 = lines[0].split(" ")#/t
    print("line1 ",line1)
    Signal = lines[1].split(" ")#/t
    #print(Signal)
    Plist = []
    for i in range (3,(Pcount+3)):# was 3 to 10
        CurrentLine = lines[i].split(" ")#/t
        Plist.append(CurrentLine)

    Bcount = (len(line1)-1)
    
    print(Pcount)
    PcountText = str(Pcount)
    print("NofChannels ", NofChannels)
    #Create the top part
    Text = "imax "
    Text = Text + str(Bcount) + " number of bins\n"#str(Bcount)
    Text = Text + "jmax " + PcountText + " number of processes minus 1\n"
    Text = Text + "kmax " + str(NoUC) + " number of nuisance parameters\n" #the str should be NoUC
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
    print("Bcount")
    print(Bcount)
    for b in range (0,Bcount):
        (WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1) = AddBin(WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1, Signal)
    Text = Text + BinLine + "\n"
    Text = Text + ProcessLine1 + "\n"
    Text = Text + ProcessLine2 + "\n"
    Text = Text + RateLine + "\n"
    Text = Text + "---------------------------------------------------------------------------------------------------------------------------------\n"


    if 0 != 0:#NoUC
        #add uncertainties
        UCtext = ""#Formated string of just the uncertainties


        UC = []#UC stands for uncertainties
        print("104")
        ###print("Input each part of the uncertainties reading left to right while including names but excluding LnN and spaces")
        for i in range (0,int(NoUC)):
            Templist = []
            print(NoUC, "108")
            for e in range (0,Bcount * Pcount + 1):
                x = input()
                Templist.append(x)
                print("111")
            UC.append(Templist)

        print("113")


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

        #Text = Text + UCtext
        #Text = Text + "\nCMS_e  lnN   -            1.3          -            -            -            "
        #Text = Text + "\nCMS_a  lnN   -            -            1.3          -            -            "
        #Text = Text + "\nCMS_b  lnN   -            -            -            1.3          -            "
        #Text = Text + "\nCMS_c  lnN   -            -            -            -            1.3          "
        #Text = Text + "\nLepID  lnN   1.05         1.05         1.05         1.05         1.05         "
        #Text = Text + "\nRenor  lnN   1.2          1.2          1.2          1.2          1.2          "
        #Text = Text + "\nTt___  lnN   -            1.3          -            -            2            "
        #Text = Text + "\nPDF__  lnN   1.37         1.37         1.37         1.37         1.37         "
    Systematics = Systematics.split("\\n")
    print(len(Systematics), "systematic list")
    for k in range(0,len(Systematics)):
        Text = Text + "\n" + Systematics[k]
    print("141")
    print(Systematics)

        #Text = Text + "\nCMS_signal  lnN   1.2            -            -            -            -     "
        #Text = Text + "\nCMS_ttbar  lnN   -            2            -            -            -              "
        #Text = Text + "\nCMS_ZZ  lnN   -            -            2            -            -             "
        #Text = Text + "\nCMS_DY  lnN   -            -            -            2            -                  "
        #Text = Text + "\nCMS_QCD  lnN   -            -            -            -            2             "
        #Text = Text + "\nCMSMuID  lnN   1.02            1.01            1.02            1.02            1.01"
        #Text = Text + "\nCMSElID  lnN   1.04            1.02            1.04            1.04            1.02            -            -            -            -            -            "
        #Text = Text + "\nCMSLumi  lnN   1.01            1.01            1.01            1.01            1.01"

    Info.close()
    b = open("datacardlist.txt", "a")
    b.write(Text)
    b.write("\n\n\n")
    b.close()
    ##print(Plist)
    ##print(Plist[0][1])
    #f = open(Title, "w")
    #f.write(Text)
    #print(Text)
    #f.close()
    return Text











def runHCT(rMaxes, rMaxIndex, NofToys):
    ###print("hello")
    Command = 'combine -M BayesianToyMC tempdatacard.txt -t '+ str(NofToys) + ' --noDefaultPrior=0 --rMax ' + str(rMaxes[rMaxIndex])
    stream = os.popen(Command)
    print(Command)
    #print("148")
    output = stream.read()
    #print("150")
    #print(output)
    outputArray = output.split("mean   expected limit: r <")
    rawResults = outputArray[1]
    ###print(rawResults)



    rawResults = rawResults.split("\n")
    MedianLine = rawResults[1]
    #print("Median")
    #print(MedianLine)
    MedianLine = MedianLine.split(" ")
    #print(MedianLine[5])


    ConfidenceRange68Line = rawResults[2]
    #print("68%")
    #print(ConfidenceRange68Line)
    ConfidenceRange68Line = ConfidenceRange68Line.split(" ")
    #print(ConfidenceRange68Line[7])
    #print(ConfidenceRange68Line[11])

    ConfidenceRange95Line = rawResults[3]
    #print("95%")
    #print(ConfidenceRange95Line)
    ConfidenceRange95Line = ConfidenceRange95Line.split(" ")
    #print(ConfidenceRange95Line[7])
    #print(ConfidenceRange95Line[11])



    Median = MedianLine[5]
    CI68 = [ConfidenceRange68Line[7],ConfidenceRange68Line[11]]
    CI95 = [ConfidenceRange95Line[7],ConfidenceRange95Line[11]]
    print("--------")
    #print(Median)
    #print(CI68)
    #print(CI95)
    results = [Median, CI68, CI95]
    return results




print("Enter 0 to create or delete different presets for studies.  Enter 1 to see a list of all current presets.  Enter a number higher to choose that specific preset.")
UserInput = input("-->")


f = open("presets.txt", "r")
presetstart = list((f.read()).split("|"))
presets = presetstart
for i in range (0,len(presetstart)):
    presets[i] = presets[i].split("~")
f.close()


if UserInput == 0:
    

    print("Enter 0 to add a new preset.  Enter 1 to remove a preset.")
    UserPresetChoice = input("-->")
    if UserPresetChoice == 0:
        print("Enter the name of the preset")
        PresetName = raw_input("-->")
        print("How many mass levels?")
        Masses = input("-->")
        print("Please list the rMax values going from lowest mass to highest mass")
        rMaxes = []
        for m in range(0,int(Masses)):
            rMaxes.append(input("-->"))
        print("How many channels per mass level?")
        Channels = input("-->")
        print("How many systemic uncertainties?")
        NoUC = input("-->")
        print("How many parameters are there?")
        NofP = input("-->")
        print("Please list off the uncertainty lines from that datacard in one line.  Include the backlash n to indicate new lines")
        Systematics = raw_input("-->")

        print(PresetName, " was added as a preset!")



        NewPreset = [PresetName, Masses, rMaxes, Channels, NoUC,NofP, Systematics]
        presets.append(NewPreset)

    if UserPresetChoice == 1:
        print("Here is the list of different presets there are.  Please input the number next to the preset you wish to remove.")
        for a in range(0,len(presets)):
            #Line = (presets[a][0]), + "    ", + str(a)
            Line = presets[a][0]
            Line = Line + "   "
            Line = Line + str(a)
            print(Line)
        removedlistindex = input("-->")
        presets.pop(removedlistindex)
        print("preset removed")
    
    
    f = open("presets.txt", "w")
    text = ""
    for a in range (0,len(presets)):
        for b in range(0,len(presets[a])):
            text = text + str(presets[a][b]) + "~"
            #print(text)
        text = text[0:len(text)-1] + "|"
    if text[0:1] == "|":
        text = text[1:len(text)-1]
    else:
        text = text[0:len(text)-1]
    f.write(text)
    f.close()
elif UserInput == 1:
    for a in range(2,len(presets)+2):
        #Line = (presets[a][0]), + "    ", + str(a)
        Line = presets[a-2][0]
        Line = Line + "   "
        Line = Line + str(a)
        print(Line)




else:#Run actual datacards
    PresetIndex = UserInput-2
    PresetName = presets[PresetIndex][0]
    NofChannels = int(presets[PresetIndex][3])
    NofMasses = int(presets[PresetIndex][1])
    Systematics = (presets[PresetIndex][6])
    rMaxes = str(presets[PresetIndex][2])
    rMaxes = rMaxes[1:len(rMaxes)-1]
    rMaxes = rMaxes.split(",")

    #print(presets[PresetIndex][5])

    NoUC = presets[PresetIndex][4]
    for l in range(0,len(rMaxes)):
        rMaxes[l] = int(rMaxes[l])
    #print(rMaxes)
    rMaxIndex = 0
    print("How many toys per Mass level?")
    NofToys = str(input("-->"))
    WhichP = []
    for o in range(0,int(presets[PresetIndex][5])):
        WhichP.append(int(o+1))


    if NofChannels == 1:
        cardName = PresetName + "tempdatacard"
        infoNameDefault = PresetName + "Info"
        TotalResults = []
        for i in range (1,NofMasses+1):#Number of different masses
            infoName = infoNameDefault + str(i) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
            datacard = makeCards(cardName,infoName,NofChannels, WhichP, NoUC, Systematics)
            f = open(cardName + ".txt", "w")
            f.write(datacard)
            f.close()
            print("running HCT")
            Results = runHCT(rMaxes, rMaxIndex, NofToys)
            TotalResults.append(Results)
            print(Results)
            rMaxIndex = rMaxIndex + 1
    else:
        cardName = PresetName + "tempdatacard"
        infoNameDefault = PresetName + "Info"
        TotalResults = []
        ChannelList = []
        
        for i in range (1,NofMasses+1):#Number of different masses
            for c in range (1, NofChannels + 1):
                infoName = infoNameDefault + str(i) +"-" + str(c) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
                datacard = makeCards(cardName,infoName,NofChannels, WhichP, NoUC, Systematics)
                f = open(cardName + str(c) + ".txt", "w")
                f.write(datacard)
                f.close()
            CombineCardCommand = "combineCards.py "
            for b in range (1, NofChannels + 1):
                CombineCardCommand = CombineCardCommand + "Card" + str(b) + "=" + cardName + str(b) + ".txt "
            
            stream = os.popen(CombineCardCommand)
            output = stream.read()
            #print(output)
            #output = output.split("imax 1 number of bins")#fix the bin count
            #print(output[0])
            #FixedOutput = output[0] + "imax " + str(NofChannels) +" number of bins" + output[1]
            
            f = open(cardName + ".txt", "w")
            f.write(output)
            f.close()

            print("running HCT")
            Results = runHCT(rMaxes, rMaxIndex, NofToys)
            TotalResults.append(Results)
            print(Results)
            rMaxIndex = rMaxIndex + 1







#f = open("presets.txt", "a")
#f.write("Now the file has more content!")
#f.close()














# print("Type 1 for Higgs, type 2 for lepton jets")
# WhichStudy = input()
# print("How many channels per mass level?")
# NofChannels = int(input())
# if WhichStudy == 1:
#     rMaxes = [0.1,0.1,0.3,1,2,5,10,15]#[0.1,1,5,10,50,100],[0.1,0.1,0.3,1,2,5,10,15]
# else:
#     rMaxes = [0.1,0.1,0.5]
# NofMasses = len(rMaxes)
# rMaxIndex = 0
# if WhichStudy == 1:
#     if NofChannels == 1:
#         cardName = "tempdatacard"
#         infoNameDefault = "DatacardInfo"
#         TotalResults = []
#         for i in range (1,NofMasses+1):#Number of different masses
#             infoName = infoNameDefault + str(i) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
#             datacard = makeCards(cardName,infoName,NofChannels)
#             f = open("tempdatacard.txt", "w")
#             f.write(datacard)
#             f.close()
#             print("running HCT")
#             Results = runHCT(rMaxes, rMaxIndex)
#             TotalResults.append(Results)
#             print(Results)
#             rMaxIndex = rMaxIndex + 1
#     else:
#         cardName = "tempdatacard"
#         infoNameDefault = "DatacardInfo"
#         TotalResults = []
#         ChannelList = []
        
#         for i in range (1,NofMasses+1):#Number of different masses
#             for c in range (1, NofChannels + 1):
#                 infoName = infoNameDefault + str(i) +"-" + str(c) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
#                 datacard = makeCards(cardName,infoName,NofChannels)
#                 f = open("tempdatacard" + str(c) + ".txt", "w")
#                 f.write(datacard)
#                 f.close()
#             CombineCardCommand = "combineCards.py "
#             for b in range (1, NofChannels + 1):
#                 CombineCardCommand = CombineCardCommand + "Card" + str(b) + "=" + "tempdatacard" + str(b) + ".txt "
            
#             stream = os.popen(CombineCardCommand)
#             output = stream.read()
#             #print(output)
#             #output = output.split("imax 1 number of bins")#fix the bin count
#             #print(output[0])
#             #FixedOutput = output[0] + "imax " + str(NofChannels) +" number of bins" + output[1]
            
#             f = open("tempdatacard.txt", "w")
#             f.write(output)
#             f.close()

#             print("running HCT")
#             Results = runHCT(rMaxes, rMaxIndex)
#             TotalResults.append(Results)
#             print(Results)
#             rMaxIndex = rMaxIndex + 1

# else:
#     cardName = "tempdatacard"
#     infoNameDefault = "DatacardLepInfo"
#     TotalResults = []
#     for i in range (1,NofMasses+1):#Number of different masses
#         infoName = infoNameDefault + str(i) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
#         datacard = makeCards(cardName,infoName,NofChannels)
#         f = open("tempdatacard.txt", "w")
#         f.write(datacard)
#         f.close()
#         print("running HCT")
#         Results = runHCT(rMaxes, rMaxIndex)
#         TotalResults.append(Results)
#         print(Results)
# ResultFile = open("RawResultFile.txt", "w")
# ResultFile.write(str(TotalResults))
#-------------------------------------------------------------

#open and read the file after the appending:
#f = open("tempdatacard.txt", "r")
#print(f.read())

















# import os


# #def valueFinder():

# def AddBin(WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1, Signal):


#     BinLine = BinLine + "Lep" + line1[b+1]
#     BinLine = BinLine + (" " * (10-len(line1[b+1])))
#     Pname = Signal[0]
#     if len(Pname) > 12:
#         Pname = Pname[0:12]
#     ProcessLine1 = ProcessLine1 + Pname
#     ProcessLine1 = ProcessLine1 + (" " * (13 - len(Pname)))
#     ProcessLine2 = ProcessLine2 + "0"
#     ProcessLine2 = ProcessLine2 + (" " * (12))
#     print("----------------")
#     print(b)
#     CurrentRate = str(Signal[b+1])
#     if len(CurrentRate) > 12:
#         CurrentRate = CurrentRate[0:12]
#     RateLine = RateLine + CurrentRate
#     RateLine = RateLine + (" " * (13 - len(CurrentRate)))

#     for i in range(0,Pcount):
#         BinLine = BinLine + "Lep" + line1[b+1]
#         BinLine = BinLine + (" " * (10-len(line1[b+1])))
#         ProcessLine1 = ProcessLine1 + Plist[WhichP[i] -1 ][0]
#         ProcessLine1 = ProcessLine1 + (" " * (13 - len(Plist[WhichP[i] -1 ][0])))
#         ProcessLine2 = ProcessLine2 + str(i+1)
#         ProcessLine2 = ProcessLine2 + (" " * (13 - len(str(i+1))))
#         CurrentRate = str(Plist[WhichP[i] -1 ][b +1 ])
#         if len(CurrentRate) > 12:
#             CurrentRate = CurrentRate[0:12]
#         RateLine = RateLine + CurrentRate
#         RateLine = RateLine + (" " * (13 - len(CurrentRate)))
#     return (WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1)



# def makeCards(cardName, infoName):
#     WhichP = [1,2,3,4] #1,2,4,5,7

#     #NoUC = input('How many uncertainties do you have?')
#     NoUC = "0"

#     ###print("Insert Datacard Title")
#     #Title = input()
#     Title = "cardName"
#     ##f = open(Title, "w")
#     Info = open(infoName, "r")
#     Parameters = Info.read()
#     lines = Parameters.split("\n")
#     ###print(lines)

#     line1 = lines[0].split(" ")#/t
#     print("line1 ",line1)
#     Signal = lines[1].split(" ")#/t
#     #print(Signal)
#     Plist = []
#     for i in range (3,7):# was 3 to 10
#         CurrentLine = lines[i].split(" ")#/t
#         Plist.append(CurrentLine)

#     Bcount = (len(line1)-1)
#     Pcount = len(WhichP)
#     print(Pcount)
#     PcountText = str(Pcount)

#     #Create the top part
#     Text = "imax "
#     Text = Text + str(Bcount) + " number of bins\n"
#     Text = Text + "jmax " + PcountText + " number of processes minus 1\n"
#     Text = Text + "kmax " + str("7") + " number of nuisance parameters\n" #the str should be NoUC
#     Text = Text + "----------------------------------------------------------------------------------------------------------------------------------\n"
#     Currentline = "bin          "
#     for i in range (1,len(line1)):
#         Currentline = Currentline + "Lep" + line1[i] + "        "
#     Text = Text + Currentline + "\n"
#     Text = Text + "observation  " + ("43.420453  " * Bcount) +"\n"
#     Text = Text + "----------------------------------------------------------------------------------------------------------------------------------\n"
#     BinLine = "bin          "
#     ProcessLine1 = "process      "
#     ProcessLine2 = "process      "
#     RateLine = "rate         "
#     print("Bcount")
#     print(Bcount)
#     for b in range (0,Bcount):
#         (WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1) = AddBin(WhichP, Plist, BinLine, ProcessLine1, ProcessLine2, RateLine, Pcount, b, line1, Signal)
#     Text = Text + BinLine + "\n"
#     Text = Text + ProcessLine1 + "\n"
#     Text = Text + ProcessLine2 + "\n"
#     Text = Text + RateLine + "\n"
#     Text = Text + "---------------------------------------------------------------------------------------------------------------------------------\n"


#     if NoUC != 0:
#         #add uncertainties
#         UCtext = ""#Formated string of just the uncertainties


#         UC = []#UC stands for uncertainties

#         ###print("Input each part of the uncertainties reading left to right while including names but excluding LnN and spaces")
#         for i in range (0,int(NoUC)):
#             Templist = []
#             for e in range (0,Bcount * Pcount + 1):
#                 x = input()
#                 Templist.append(x)
#             UC.append(Templist)



#         #format list to datacard

#         Method = "lnN     "#Choose which method is used

#         for i in range (0,int(NoUC)):
#             UCtext = UCtext + UC[i][0] + (" " * (19 - len(UC[i][0])))#add name of uncertainty
#             UCtext = UCtext + Method
#             for e in range (1,Bcount * Pcount + 1):
#                 CUC = UC[i][e]
#                 CUC = CUC + (" " * (16 - len(CUC)))
#                 UCtext = UCtext + CUC
#             UCtext = UCtext + "\n"

#         #Text = Text + UCtext
#         #Text = Text + "\nCMS_e  lnN   -            1.3          -            -            -            "
#         #Text = Text + "\nCMS_a  lnN   -            -            1.3          -            -            "
#         #Text = Text + "\nCMS_b  lnN   -            -            -            1.3          -            "
#         #Text = Text + "\nCMS_c  lnN   -            -            -            -            1.3          "
#         #Text = Text + "\nLepID  lnN   1.05         1.05         1.05         1.05         1.05         "
#         #Text = Text + "\nRenor  lnN   1.2          1.2          1.2          1.2          1.2          "
#         #Text = Text + "\nTt___  lnN   -            1.3          -            -            2            "
#         #Text = Text + "\nPDF__  lnN   1.37         1.37         1.37         1.37         1.37         "

#         Text = Text + "\nCMS_signal  lnN   1.2            -            -            -            -     "
#         Text = Text + "\nCMS_ttbar  lnN   -            2            -            -            -              "
#         Text = Text + "\nCMS_ZZ  lnN   -            -            2            -            -             "
#         Text = Text + "\nCMS_DY  lnN   -            -            -            2            -                  "
#         Text = Text + "\nCMS_QCD  lnN   -            -            -            -            2             "
#         Text = Text + "\nCMSMuID  lnN   1.02            1.01            1.02            1.02            1.01"
#         #Text = Text + "\nCMSElID  lnN   1.04            1.02            1.04            1.04            1.02            -            -            -            -            -            "
#         Text = Text + "\nCMSLumi  lnN   1.01            1.01            1.01            1.01            1.01"

#     Info.close()
#     b = open("datacardlist.txt", "a")
#     b.write(Text)
#     b.write("\n\n\n")
#     b.close()
#     ##print(Plist)
#     ##print(Plist[0][1])
#     #f = open(Title, "w")
#     #f.write(Text)
#     #print(Text)
#     #f.close()
#     return Text











# def runHCT(rMaxes, rMaxIndex):
#     ###print("hello")
#     Command = 'combine -M BayesianToyMC tempdatacard.txt -t 2000 --noDefaultPrior=0 --rMax ' + str(rMaxes[rMaxIndex])
#     stream = os.popen(Command)
#     print(Command)
#     print("148")
#     output = stream.read()
#     print("150")
#     #print(output)
#     outputArray = output.split("mean   expected limit: r <")
#     rawResults = outputArray[1]
#     ###print(rawResults)



#     rawResults = rawResults.split("\n")
#     MedianLine = rawResults[1]
#     #print("Median")
#     #print(MedianLine)
#     MedianLine = MedianLine.split(" ")
#     #print(MedianLine[5])


#     ConfidenceRange68Line = rawResults[2]
#     #print("68%")
#     #print(ConfidenceRange68Line)
#     ConfidenceRange68Line = ConfidenceRange68Line.split(" ")
#     #print(ConfidenceRange68Line[7])
#     #print(ConfidenceRange68Line[11])

#     ConfidenceRange95Line = rawResults[3]
#     #print("95%")
#     #print(ConfidenceRange95Line)
#     ConfidenceRange95Line = ConfidenceRange95Line.split(" ")
#     #print(ConfidenceRange95Line[7])
#     #print(ConfidenceRange95Line[11])



#     Median = MedianLine[5]
#     CI68 = [ConfidenceRange68Line[7],ConfidenceRange68Line[11]]
#     CI95 = [ConfidenceRange95Line[7],ConfidenceRange95Line[11]]
#     print("--------")
#     #print(Median)
#     #print(CI68)
#     #print(CI95)
#     results = [Median, CI68, CI95]
#     return results


# print("Type 1 for Higgs, type 2 for lepton jets")
# WhichStudy = input()
# if WhichStudy == 1:
#     rMaxes = [0.1,0.1,0.3,1,2,5,10,15]#[0.1,1,5,10,50,100],[0.1,0.1,0.3,1,2,5,10,15]
# else:
#     rMaxes = [0.1,0.1,0.5]
# rMaxIndex = 0
# if WhichStudy == 1:
#     cardName = "tempdatacard"
#     infoNameDefault = "DatacardInfo"
#     TotalResults = []
#     for i in range (1,8+1):#Number of different masses
#         infoName = infoNameDefault + str(i) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
#         datacard = makeCards(cardName,infoName)
#         f = open("tempdatacard.txt", "w")
#         f.write(datacard)
#         f.close()
#         print("running HCT")
#         Results = runHCT(rMaxes, rMaxIndex)
#         TotalResults.append(Results)
#         print(Results)
#         rMaxIndex = rMaxIndex + 1

# else:
#     cardName = "tempdatacard"
#     infoNameDefault = "DatacardLepInfo"
#     TotalResults = []
#     for i in range (1,3+1):#Number of different masses
#         infoName = infoNameDefault + str(i) + ".txt" #access datacardinfo1.txt then datacardinfo2.txt
#         datacard = makeCards(cardName,infoName)
#         f = open("tempdatacard.txt", "w")
#         f.write(datacard)
#         f.close()
#         print("running HCT")
#         Results = runHCT(rMaxes, rMaxIndex)
#         TotalResults.append(Results)
#         print(Results)
# ResultFile = open("RawResultFile.txt", "w")
# ResultFile.write(str(TotalResults))


# #open and read the file after the appending:
# #f = open("tempdatacard.txt", "r")
# #print(f.read())





