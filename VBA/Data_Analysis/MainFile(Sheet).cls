VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "Sheet1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
Private Sub AnalyseBtn_Click()
    Dim InfoCode As Integer, StartDate As Date, EndDate As Date
    
    ' PURPOSE : Configurations
    Module1.iStartingRow = 3
    Module1.iStartingColomn = 4
    Module1.inumMics = 30
    
    ' PURPOSE : Collecting Data from Sheet
    StartDate = Range("B2")
    EndDate = Range("E2")
    Module1.TimeInterval = Range("E1")
        
    ' PURPOSE : Initialization
    NoUse = Module1.Initialize()
    
    ' PURPOSE : Calculations
    Module1.inumDays = Application.WorksheetFunction.RoundUp(EndDate - StartDate, 0)
    
    ' PURPOSE : Loading Data
    InfoCode = LoadMatrix()
        
    Select Case (InfoCode)
        Case 0
            Beep
            Application.StatusBar = "Error Loading Data - Terminating analysis"
            Exit Sub
        Case 1
            Beep
            Application.StatusBar = "Done Loading Data"
            
    End Select
        
    ' PURPOSE : Analysing Data
    NoUse = TestforSpikes()
        
    Application.StatusBar = "Calculations has been completed!"
        
    ' PURPOSE : Displaying Data
    NoUse = DisplayData()
    
    Application.StatusBar = ""
    Beep
        
End Sub

Private Sub UpdateBtn_Click()
    ' PURPOSE : adds the data on the Data Analysis to the Data sheet all at one time
    '           this lets PI only access the servers to pull data once, thus increasing performance
    
    Application.StatusBar = "PI Loading Data"
    
    Sheets("Data").Range("B1") = Sheets("Data Analysis").Range("B1")
    Sheets("Data").Range("B2") = Sheets("Data Analysis").Range("B2")
    Sheets("Data").Range("B3") = Sheets("Data Analysis").Range("E2")
    Sheets("Data").Range("B6") = Sheets("Data Analysis").Range("E1") & "s"
    
    Application.StatusBar = "Ready"
End Sub
