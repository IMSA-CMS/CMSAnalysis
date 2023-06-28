import os



def CutText(Text, Target):
    
    A = Text.split(Target)
    B = ""
    for i in range (0,len(A)):
        B = B + A[i]
    return B









Name = "DatacardInfoEE"
#Name2 = input("Enter a name extension for the info sheets -->")
#Name = Name + Name2

f = open("InsertData.txt", "r")
RawData = f.read()
CutData = RawData


#RawData = RawData.split("\n\n")
#print(RawData)
CutData = CutText(CutData, "Inclusive ")
CutData = CutText(CutData, "(Dilepton)")
CutData = CutText(CutData, "++ Signal")
#print(CutData)

CutData = CutData.split("\n\n")
#print(CutData)
for e in range (0,len(CutData)):
    A = CutData[e].split("\n")
    B = ""
    for i in range (0,2):
        B = B + A[i] + "\n"
    B = B + "\n"
    for i in range (2,len(A)):
        B = B + A[i] + "\n"
    CutData[e] = B
#print(CutData)


for i in range (0,len(CutData)):
    f = open(Name + str(i+1) +".txt", "w")
    f.write(CutData[i])
    f.close()

#Add the space after signal




#SemiCutData = []
#for i in range(0,len(RawData)):
#    Line = RawData[i].split("\n")
#    SemiCutData.append(Line)
#print(SemiCutData)
#for i in range(0,len(SemiCutData)):
#    Test = []
#    for e in range(0,len(SemiCutData[i])):
#        Line = SemiCutData[i][e].split(" ")
#        Test.append(Line)
#    CutData.append(Test)
#NofP = len(CutData[1])-1 #Includes H++
#print(NofP)
#if CutData[0][0][0] == "Electron":
#    Case = "EE"
#elif CutData[0][0][0] == "Muon":
#    Case = "Mu"
#print(Case)
#print(CutData)
