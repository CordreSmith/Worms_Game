Attribute VB_Name = "Module1"
' PURPOSE : Local Variables

Dim Microphones() As Variant    ' DEFINITION : Matrix to store state of Microphones
                                ' DATA DEFINITION : 1 = Working
                                ' DATA DEFINITION : 0 = Out of Order
Dim SootBlowing() As Variant    ' DEFINITION : Matrix to store Soot Bowling data
                                ' DATA DEFINITION : On
                                ' DATA DEFINITION : Off
Dim Units() As Variant          ' DEFINITION : Matrix storing data on the Maintenance done on each Microphone
Dim Data() As Variant           ' DEFINITION : Matrix storing main data used
Dim Load() As Variant           ' DEFINITION : Matrix storing the load data in Percentage

Dim MaintData() As Integer      ' DEFINITION : Matrix to store the Maintenance data of each Microphone
                                ' DATA DEFINTION 1 = Maintenance Occured
                                ' DATA DEFINTION 2 = No Maintenance Occured
                                ' DATA DEFINTION 3 = Spike occured but data reading didnt change
                                ' DATA DEFINTION 4 = Microphone is Out of Order
                                ' DATA DEFINTION 5 = Unit Down

Dim DataPointsDay As Long       ' DEFINITION : The amount of datapoints per day we are working with this analysis

' PURPOSE : Public Variables

Public iStartingColomn As Long  ' DEFINITION : Defines the starting colomn
Public inumDataPoints As Long   ' DEFINITION : Defines the number of Datapoints we are working with
Public TimeInterval As Long     ' DEFINITION : Defines the TimeInterval used by the PI server
Public iStartingRow As Long     ' DEFINITION : Defines the starting row
Public inumMics As Long         ' DEFINITION : Defines amount of Microphones in the system

Public inumDays As Double ' DEFINITION : Defines the amount of days the analysis is running for

' PURPOSE : Constants

Const SecondsinDay = 86400  ' DEFINTION : Seconds in 1 Day = 86400


Function Initialize() As Long
    
    ' PURPOSE : Defines the amount of datapoints per day calculation
    DataPointsDay = SecondsinDay / TimeInterval
    
    ' PURPOSE : Assume all Microphones as working from the Start
    ReDim Microphones(0 To (inumMics - 1))
    
    For i = 0 To inumMics - 1
        Microphones(i) = 1
    Next i
    
    ' PURPOSE : Clear content on screen
    
    Sheets("Data Analysis").Range("A9:AZ49").Select
    Selection.Clear
    Sheets("Data Analysis").Range("A1").Select
    
End Function


Function LoadMatrix() As Long
    ' PURPOSE : Initialize
    Dim inumRows As Long
    inumRows = iStartingRow

    ' PURPOSE : Error Handler & Status Displaying
    On Error GoTo ErrHandler:
    Application.StatusBar = "Loading data"
    
    ' PURPOSE : Calculating amount of Datapoints needed
    Do While 1
        If ((Sheets("Data").Cells(inumRows + 1, iStartingColomn).Value = " ") Or _
            (Sheets("Data").Cells(inumRows + 1, iStartingColomn).Value = "")) Then
           Exit Do
        ElseIf (IsNumeric(Sheets("Data").Cells(inumRows + 1, iStartingColomn).Value) = True) Then
            inumRows = inumRows + 1
        Else
            MsgBox "Error 001 - Found something irregular while loading data"
            LoadMatrix = 0
            Exit Function
        End If
    Loop
    
    inumDataPoints = inumRows - iStartingRow + 1
    
    ' PURPOSE : Load Data in Matrix
    ReDim Data(inumDataPoints - 1, inumMics - 1)
    ReDim Load(inumDataPoints - 1)
    ReDim SootBlowing(inumDataPoints - 1)
    
    For row = 0 To (inumRows - iStartingRow)
        For col = 0 To (inumMics - 1)
            ' PURPOSE : Firstly testing to see if the Microphone is working
            If (Microphones(col) = 1) Then
                Data(row, col) = Sheets("Data").Cells(row + iStartingRow, col + iStartingColomn).Value
            Else
                Data(row, col) = 0
            End If
        Next col
        Load(row) = (Sheets("Data").Cells(row + iStartingRow, col + iStartingColomn).Value / 686) * 100
        SootBlowing(row) = (Sheets("Data").Cells(row + iStartingRow, col + iStartingColomn + 1).Value)
    Next row
    
    LoadMatrix = 1
    
    Exit Function
    
    ' PURPOSE : Error Handler
    
ErrHandler:
        LoadMatrix = 0
        MsgBox "Error Occured in Data Loader"
        Application.StatusBar = "Error Occured in Data Loader"

End Function

Function TestforSpikes() As Long

    Dim UnitStartTime As Long   ' DEFINTION : Variable for data of the time when the Units comes back online after an outage
    Dim SpikeStart() As Long    ' DEFINTION : Matrix for starting time of Spikes
    Dim SpikeEnd() As Long      ' DEFINTION : Matrix for Ending time of Spikes
    Dim numSpikes As Long       ' DEFINTION : Variable for data on the amount of spikes that have occured
    Dim curDay As Long          ' DEFINTION : Variable for holding the current day
    
    Dim Infonum As Integer      ' DEFINTION : Variable for data on what Maintenance occured on the specific Microphone
    
    Dim SootBFlag As Boolean    ' DEFINITION : Flag for knowing if sootblowing is on or not
    Dim UnitDown As Boolean     ' DEFINITION : Flag for saying the Unit is down
    Dim Spiked As Boolean       ' DEFINITION : Flag for syaing a spike has occured

    Const Secondsin5Min = 300   ' DEFINITION : Seconds in 5 Minutes = 300
     
    ' PURPOSE : Redefining the Dimensions of the Units Array to fit the Number of Mics we have
    
    ReDim Units(inumMics - 1, inumDays)
    
    Application.StatusBar = "Analysing Data"
    
    For colom = 0 To inumMics - 1
        UnitDown = False
        UnitStartTime = -1
        
        ' PURPOSE : Testing if Microphone are set as Out of Order
        If (Microphones(colom) = 1) Then
            For row = 0 To inumDataPoints - 1
                curDay = Application.WorksheetFunction.RoundUp((row - 1) / DataPointsDay, 0)
            
                ' PURPOSE : Testing if it is end of the Day
                    
                If ((((row + 1) / DataPointsDay) - Int((row + 1) / DataPointsDay)) = 0 Or (row = inumDataPoints - 1)) Then
                         
                    If (UnitDown = True) Then
                        Infonum = 6
                    ElseIf (numSpikes = 0) Then
                        Infonum = testNoSpike(colom, curDay)
                    ElseIf (numSpikes > 0) Then
                        Infonum = testfoundSpikes(colom, SpikeStart(), SpikeEnd(), numSpikes, curDay)
                    End If
                            
                    Select Case Infonum
                    Case 1
                        Units(colom, curDay - 1) = "Maintenanced"
                    Case 2
                        Units(colom, curDay - 1) = "No Change"
                    Case 3
                        Units(colom, curDay - 1) = "Low levels"
                    Case 4
                        Units(colom, curDay - 1) = "High levels"
                    Case 5
                        Units(colom, curDay - 1) = "Normal levels"
                    Case 6
                        Units(colom, curDay - 1) = "Unit is Down"
                    Case 7
                        Units(colom, curDay - 1) = "Soot Blowing"
                    End Select
                            
                    Spiked = False
                    numSpikes = 0
                            
                End If
                            
                If (SootBlowing(row) = "ON" And SootBFlag = False) Then
                    SootBFlag = True
                ElseIf (SootBlowing(row) = "Off" And SootBFlag = True) Then
                   Dim SBrestarted As Boolean
                    For i = 1 To (Secondsin5Min / TimeInterval)
                        If (row + i) < inumDataPoints Then
                            If (SootBlowing(row + i) = "ON") Then
                                SBrestarted = True
                                Exit For
                            End If
                        End If
                    Next i
                
                   If (SBrestarted = False) Then
                        SootBFlag = False
                    End If
                ElseIf (SootBFlag = False) Then
                    
                    ' PURPOSE : Testing if an Spike occured
                    Application.StatusBar = "Analysing Data : Testing for a Spike"
                    
                    ' PURPOSE : First testing if the unit is not down
                    If (UnitDown = False And Load(row) <= 1.5) Then
                        UnitDown = True
                        
                    ' PURPOSE : Secondly testing if the unit has started up again after unit down
                    ElseIf (UnitDown = True And Load(row) > 1.5) Then
                        If (UnitStartupTime = -1) Then
                            UnitStartupTime = row
                            
                            ' PURPOSE : This is to give the unit an 24 Hour grace period after startup before starting to read data again
                        ElseIf ((row - UnitStartupTimeR) >= DataPointsDay) Then
                            UnitDown = False
                            UnitStartTime = -1
                        End If
                    
                    ' PURPOSE : If Unit is not test for Spikes
                    ElseIf (Load(row) > 1.5 And UnitDown = False) Then
                    
                        ' PURPOSE : Testing for a spike , will be considered spike if value goes above 60
                        If (Data(row, colom) >= 60) Then
                            If (Spiked = False) Then
                                
                                Spiked = True
                                ReDim Preserve SpikeStart(0 To numSpikes)
                                SpikeStart(numSpikes) = row
                                
                            End If
                        Else
                            If (Spiked = True) Then
                                ' PURPOSE : Testing to see if the spike does not continue within the next 2 minutes and if the spike
                                '           reoccures then accepting that as the same spike
                                Dim ReSpike As Boolean
                                ReSpike = False
                                For i = 1 To (Secondsin5Min / TimeInterval)
                                    If (Data(row + i, colom) > 60) Then
                                        ReSpike = True
                                        Exit For
                                    End If
                                Next i
                                
                                ' PURPOSE : If no reoccuring spike is detected the ending of the spike is stored and Number of Spikes is increased
                                If (ReSpike = False) Then
                                    ReDim Preserve SpikeEnd(0 To numSpikes)
                                    SpikeEnd(numSpikes) = row - 1
                                    Spiked = False
                                    numSpikes = numSpikes + 1
                                End If
                                
                            End If
                        End If
                    End If
                End If
            Next row
        Else
            For D = 1 To inumDays
                Units(colom, D - 1) = "Out of Order"
            Next D
        End If
    Next colom
    
    TestforSpikes = 1
End Function

Function testfoundSpikes(ByVal MicNum As Long, ByRef SpikeStart() As Long, ByRef SpikeEnd() As Long, ByVal numSpikes As Long, ByVal DayNum As Long) As Long
    Dim LDataArr() As Double
    Dim HDataArr() As Double
    Dim LAve As Double
    Dim HAve As Double
    Dim r As Long
    
    Const SecondsforMaint = 180 ' DEFINITION : 180 Seconds = 3 Minutes, this constant was chosen by doing so research as to how long Maintenance usually takes
    Const SecondsinHour = 3600  ' DEFINITION : Seconds in an Hour
    
    ' PURPOSE : Error Handler & Status Displaying

    'On Error GoTo ErrHandler:
    Application.StatusBar = "Analysing Data : Testing Spikes found"
        
    For i = 0 To numSpikes - 1

        ' PURPOSE : Takes the data 720 Datapoints, or 6 Hours worth, to the left and right of the spike
        '           and check to see if Maintenance has occured and if there was a change in readings
        ReDim HDataArr(0 To ((6 * (SecondsinHour / TimeInterval)) - 1))
        ReDim LDataArr(0 To ((6 * (SecondsinHour / TimeInterval)) - 1))
        
        For r = 0 To ((6 * (SecondsinHour / TimeInterval)) - 1)
            If ((SpikeStart(i) - 1 - r) > 0) Then
                LDataArr(r) = Data(SpikeStart(i) - 1 - r, MicNum)
            ElseIf ((SpikeStart(i) - 1 - r) = 0) Then
                ReDim Preserve LDataArr(r)
                LDataArr(r) = Data(SpikeStart(i) - 1 - r, MicNum)
            End If
            If (SpikeEnd(i) + 1 + r < inumDataPoints - 1) Then
                HDataArr(r) = Data(SpikeEnd(i) + 1 + r, MicNum)
            ElseIf (SpikeEnd(i) + 1 + r = inumDataPoints - 1) Then
                ReDim Preserve HDataArr(r)
                HDataArr(r) = Data(SpikeEnd(i) + 1 + r, MicNum)
            End If
        Next r
        
        ' PURPOSE : Calculated average of Hours worth of data left and right of spike
        LAve = Application.WorksheetFunction.Average(LDataArr)
        HAve = Application.WorksheetFunction.Average(HDataArr)
        
        
        ' PURPOSE : Uses this Data to calculate what the spike did
        '           if Average of Before was less than 12 and after between 12 and 45 then Maintenance has occured
        '           if Average of Before was less than 12 and after was less than 12 there was a spike but no change
        '           if Average of Before was between 12 and 45 and after between 12 and 45 then Maintenance has occured
        '           if Average of Before was higher than 45 and was higher than 45 then there was a spike but no change
        '           if Average of Before was higher than 45 and after was between 12 and 45 then Maintenance has occured
        '           if Average of Before was higher than 12 and after lower than 12 then there was a spike but no change
        If ((LAve <= 12) And (HAve > 12 And HAve < 45)) Then
            testfoundSpikes = 1 'Maintenance has occured'
            Exit For
        ElseIf ((LAve <= 12) And (HAve <= 12)) Then
            testfoundSpikes = 2 'There was a spike that looks like Maintenance but no change in readings, readings still too low'
        ElseIf ((LAve > 12 And LAve < 45) And (HAve > 12 And HAve < 45)) Then
            testfoundSpikes = 1 'Maintenance has occured'
            Exit For
        ElseIf ((LAve > 45) And (HAve > 45)) Then
            testfoundSpikes = 2 'There was a spike that looks like Maintenance but no change in readings, readings still too high'
        ElseIf ((LAve > 45) And (HAve > 12 And HAve <= 45)) Then
            testfoundSpikes = 1 'Maintenance has occured'
            Exit For
        ElseIf ((LAve > 12) And (HAve <= 12)) Then
            testfoundSpikes = 2 'There was a spike that looks like Maintenance but no change in readings, readings still too low'
        End If
    Next i
    
    Exit Function
    
ErrHandler:
        MsgBox "There was an Error, With r = " & r
        
End Function

Function testNoSpike(ByVal MicNum As Long, ByVal DayNum As Long) As Long
    Dim SlicedDataArr() As Double
    Dim Ave As Double
    
    Dim OneUse As Long
    Dim r As Long
    
    Application.StatusBar = "Analysing Data : Testing No Spikes found"
    
    ' PURPOSE : Getting the data for the day
    '           there is also checks in to check if the total data is not smaller than a day thus changing the size of the arrays
    If (DataPointsDay <= inumDataPoints) Then
        ReDim SlicedDataArr(0 To (DataPointsDay - 1))
        For r = 0 To (DataPointsDay - 1)
            OneUse = r + (DataPointsDay * (DayNum - 1))
            SlicedDataArr(r) = Data(OneUse, MicNum)
        Next r
    ElseIf (DataPointsDay > inumDataPoints) Then
        ReDim SlicedDataArr(0 To (inumDataPoints - 1))
        For r = 0 To (inumDataPoints - 1)
            SlicedDataArr(r) = Data(r, MicNum)
        Next r
    End If
    
    ' PURPOSE : Calculating the average of the data for day/all the data
    Ave = Application.WorksheetFunction.Average(SlicedDataArr)
    
    ' PURPOSE : Using the average that was calculated to determine how the data looks like
    If (Ave >= 0 And Ave <= 12) Then
        testNoSpike = 3 'Low levels on microphones but no maintenance'
    ElseIf (Ave > 12 And Ave <= 45) Then
        testNoSpike = 5 'Normal levels on microphones but no maintenance'
    ElseIf (Ave > 45 And Ave <= 100) Then
        testNoSpike = 4 'High levels on microphones but no maintenance'
    Else
        MsgBox ("ERROR 002 - Error with No Spike testing!")
    End If
    
End Function

Function DisplayData() As Long
    Dim NoUse As Long
    ReDim MaintData(1 To 6, 1 To inumDays)
    
    For i = 1 To 6
        For j = 1 To inumDays
            MaintData(i, j) = 0
        Next j
    Next i
    
    Application.StatusBar = "Displaying Data"
    
    ' PURPOSE : Writing some information at the top of the Data displayed
    ' DEFINITION : Maintenanced = Green = RGB(0,255,0)
    ' DEFINITION : No Maintenanced = Red = RGB(255,0,0)
    ' DEFINITION : Spike but no Change = Purple = RGB(255,0,255)
    ' DEFINITION : Out of order = Grey = RGB(200,200,200)
    Sheet1.Range("A9:B9").Merge
    Sheet1.Cells(9, 1).Interior.Color = RGB(0, 255, 0)
    Sheet1.Cells(9, 1) = "Maintenanced"
    Sheet1.Range("C9:D9").Merge
    Sheet1.Cells(9, 3).Interior.Color = RGB(255, 0, 255)
    Sheet1.Cells(9, 3) = "Spike but no Change"
    Sheet1.Range("E9:F9").Merge
    Sheet1.Cells(9, 5).Interior.Color = RGB(255, 0, 0)
    Sheet1.Cells(9, 5) = "No Maintenance"
    Sheet1.Range("G9:H9").Merge
    Sheet1.Cells(9, 7).Interior.Color = RGB(200, 200, 200)
    Sheet1.Cells(9, 7) = "Out of Order"
    
    ' PURPOSE : Running through all the Mics and Days and displaying there data together with a color
    ' DEFINITION : Maintenanced = Green = RGB(0,255,0)
    ' DEFINITION : No Maintenanced = Red = RGB(255,0,0)
    ' DEFINITION : Spike but no Change = Purple = RGB(255,0,255)
    ' DEFINITION : Out of order = Grey = RGB(200,200,200)
    For i = 1 To (inumMics)
        For j = 0 To (inumDays)
    
            ' PURPOSE : On day 0 adding Microphones labels and Calculated data labels
            If (j = 0) Then
                If (i <= 5) Then
                    Sheet1.Cells(i + 13, j + 1) = "Microphone" & i
                Else
                    Sheet1.Cells(i + 13, j + 1) = "Microphone" & (i + 1)
                End If
                
                If (i = inumMics) Then
                    Sheet1.Cells(i + 15, j + 1) = "Maintenanced"
                    Sheet1.Cells(i + 15, j + 1).Interior.Color = RGB(0, 255, 0)
                    Sheet1.Cells(i + 16, j + 1) = "No Maintenanced"
                    Sheet1.Cells(i + 16, j + 1).Interior.Color = RGB(255, 0, 0)
                    Sheet1.Cells(i + 17, j + 1) = "Spiked but no Change"
                    Sheet1.Cells(i + 17, j + 1).Interior.Color = RGB(255, 0, 255)
                    Sheet1.Cells(i + 18, j + 1) = "Out of Order"
                    Sheet1.Cells(i + 18, j + 1).Interior.Color = RGB(200, 200, 200)
                    Sheet1.Cells(i + 19, j + 1) = "Problem"
                    Range("A49").AddComment
                    Range("A49").Comment.Visible = False
                    Range("A49").Comment.Text Text:= _
                                    "There is a problem in the spike detection software refer this to the technician"
                End If
                            
            ' PURPOSE : If it is the first Microphone the day labels are added above data
            ElseIf (i = 1) Then
                Sheet1.Cells(i + 12, j + 1) = "Day " & j
                Sheet1.Cells(i + 13, j + 1) = Units(i - 1, j - 1)
                NoUse = ColourFormat(i, j)
                
            ' PURPOSE : Maintenance data is added for each Microphone for each day
            Else
                Sheet1.Cells(i + 13, j + 1) = Units(i - 1, j - 1)
                NoUse = ColourFormat(i, j)
            End If
            
        Next j
    Next i
   
    ' PURPOSE : For the number of days there calculations are done
    '           Total Mics maintenaced are calculated
    '           Total Mics not maintenaced are calculated
    '           Total Mics that had a spike with no change in data is calculated
    '           Total Mics that is out of order is shown
    For D = 1 To inumDays
        Dim total As Long
        total = 0
        For mn = 1 To 6
            total = total + MaintData(mn, D)
        Next mn
        If (total <> inumMics) Then
            MsgBox ("Error 1 - Refer this error code to a technician.")
            Exit Function
        Else
            Sheet1.Cells(inumMics + 15, 1 + D) = Application.WorksheetFunction.Round((MaintData(1, D) / (inumMics - MaintData(4, D))) * 100, 3) & " %"
            Sheet1.Cells(inumMics + 16, 1 + D) = Application.WorksheetFunction.Round((MaintData(2, D) / (inumMics - MaintData(4, D))) * 100, 3) & " %"
            Sheet1.Cells(inumMics + 17, 1 + D) = Application.WorksheetFunction.Round((MaintData(3, D) / (inumMics - MaintData(4, D))) * 100, 3) & " %"
            Sheet1.Cells(inumMics + 18, 1 + D) = MaintData(4, D)
            Sheet1.Cells(inumMics + 19, 1 + D) = MaintData(5, D)
        End If
    Next D
End Function

Function ColourFormat(ByVal UnitNum As Long, ByVal DayNum As Long) As Long

    ' PURPOSE : This function is used to displayed the certain colours with certain Statuses as said in DisplayData function
    If (Units(UnitNum - 1, DayNum - 1) = "Maintenanced") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(0, 255, 0) ' Maintenanced = Green
        MaintData(1, DayNum) = MaintData(1, DayNum) + 1
    ElseIf (Units(UnitNum - 1, DayNum - 1) = "No Change") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(255, 0, 255) ' Spike but no Change = Purple
        MaintData(3, DayNum) = MaintData(3, DayNum) + 1
    ElseIf (Units(UnitNum - 1, DayNum - 1) = "Low levels") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(255, 0, 0) ' Low levels but no Maintenance = Red
        MaintData(2, DayNum) = MaintData(2, DayNum) + 1
    ElseIf (Units(UnitNum - 1, DayNum - 1) = "Normal levels") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(255, 0, 0) ' Normal levels but no Maintenance = Red
        MaintData(2, DayNum) = MaintData(2, DayNum) + 1
    ElseIf (Units(UnitNum - 1, DayNum - 1) = "High levels") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(255, 0, 0) ' High levels but no Maintenance = Red
        MaintData(2, DayNum) = MaintData(2, DayNum) + 1
    ElseIf (Units(UnitNum - 1, DayNum - 1) = "Out of Order") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(200, 200, 200) ' Out of Order = Grey
        MaintData(4, DayNum) = MaintData(4, DayNum) + 1
    ElseIf (Units(UnitNum - 1, DayNum - 1) = "Unit is Down") Then
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.Color = RGB(0, 0, 255) ' Unit is down = Blue
        MaintData(5, DayNum) = MaintData(5, DayNum) + 1
    Else
        Sheet1.Cells(UnitNum + 13, DayNum + 1).Interior.ColorIndex = xlColorIndexNone
        MaintData(6, DayNum) = MaintData(6, DayNum) + 1
    End If
            
End Function
