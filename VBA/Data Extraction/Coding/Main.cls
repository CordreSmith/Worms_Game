VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "Sheet1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = True
'------------------------------------------'
' Created and Compiled by CORDRE SMITH     '
' on 19 January 2017                       '
' If you have any queries or Errors you    '
' can contact me at smithcordre@gmail.com  '
' If any alterations was done to this code '
' without my consent I take ZERO           '
' responsibility                           '
'------------------------------------------'

Public sDate As String, sEvent As String, sPlace As String
Option Compare Text


Private Sub ClearBtn_Click()
    Sheet1.UsedRange.Clear
    Sheet1.Range("A1") = "Date"
    Sheet1.Range("B1") = "Event"
    Sheet1.Range("C1") = "Location"
    Range("A1:C1").Select
    Selection.Style = "Check Cell"
    Selection.Font.Size = 12
    Selection.Font.Size = 14
    With Selection
        .HorizontalAlignment = xlCenter
        .VerticalAlignment = xlCenter
        .WrapText = False
        .Orientation = 0
        .AddIndent = False
        .IndentLevel = 0
        .ShrinkToFit = False
        .ReadingOrder = xlContext
        .MergeCells = False
    End With
End Sub

Private Sub GetDatabtn_Click()
    Dim sFileName As String, sTextLine(0 To 5) As String
    Dim SkippedLinesatStart As Integer, TextTestReturn As Integer, EmptyRow As Integer
    Dim iEndofEntry
    
    ' Configuration
        SkippedLinesatStart = 6
    ' Configuration END
    
    
    sFileName = Application.GetOpenFilename()
    
    On Error GoTo ErrHandler:
    With New Scripting.FileSystemObject
        With .OpenTextFile(sFileName, ForReading)
        
            '   Skipping first lines of no Info
            For i = 1 To SkippedLinesatStart
                If Not .AtEndOfStream Then .SkipLine
            Next i
            
            Do Until .AtEndOfStream
                sTextLine(0) = .ReadLine
                TextTestReturn = Module1.TestOccurance(sTextLine(0))
                Select Case TextTestReturn
                    Case 1:
                        sTextLine(1) = .ReadLine
                        sTextLine(2) = .ReadLine
                        sTextLine(3) = .ReadLine
                        
                        iEndofEntry = TestEndOfLogEntry(sTextLine(3))
                        If (iEndofEntry = 1) Then
                            If Not .AtEndOfStream Then
                                sTextLine(4) = .ReadLine
                                iEndofEntry = TestEndOfLogEntry(sTextLine(4))
                            Else
                                iEndofEntry = 0
                            End If
                            If (iEndofEntry = 0) Then
                                NoUse = Module1.ActionTest(sTextLine(1), sTextLine(2), sTextLine(3))
                            Else
                                If Not .AtEndOfStream Then
                                    sTextLine(5) = .ReadLine
                                Else
                                    sTextLine(5) = "No Data"
                                End If
                                If Not .AtEndOfStream Then .SkipLine
                                NoUse = Module1.ActionTest(sTextLine(1), sTextLine(2), sTextLine(3), sTextLine(4), sTextLine(5))
                            End If
                        Else
                            sTextLine(3) = "No Data"
                        End If
                        
                    Case 2:
                        sTextLine(1) = .ReadLine
                        If (InStr(sTextLine(1), "Zone") <> 0) Then
                            sTextLine(2) = .ReadLine
                            sTextLine(3) = .ReadLine
                            sTextLine(4) = .ReadLine
                            sTextLine(5) = .ReadLine
                            
                            iEndofEntry = TestEndOfLogEntry(sTextLine(5))
                            
                            If (iEndofEntry = 0) Then
                                sTextLine(5) = "No Data"
                            Else
                                If Not .AtEndOfStream Then .SkipLine
                            End If
                            
                            NoUse = Module1.FaultTest(sTextLine(1), sTextLine(2), sTextLine(3), sTextLine(4), sTextLine(5))
                        Else
                            sTextLine(2) = .ReadLine
                            sTextLine(3) = .ReadLine
                            
                            iEndofEntry = TestEndOfLogEntry(sTextLine(3))
                            
                            If (iEndofEntry = 0) Then
                                sTextLine(3) = "No entry"
                            Else
                                If Not .AtEndOfStream Then .SkipLine
                            End If
                            
                            NoUse = Module1.FaultTest(sTextLine(1), sTextLine(2), sTextLine(3))
                        End If
                    Case 3:
                        sTextLine(1) = .ReadLine
                        If (InStr(sTextLine(1), "Zone") <> 0) Then
                            sTextLine(2) = .ReadLine
                            sTextLine(3) = .ReadLine
                            
                            If Not .AtEndOfStream Then
                                sTextLine(4) = .ReadLine
                                iEndofEntry = TestEndOfLogEntry(sTextLine(4))
                            Else
                                iEndofEntry = 0
                            End If
                                                    
                            If (iEndofEntry = 0) Then
                                sTextLine(4) = "No Entry"
                                sTextLine(5) = "No Entry"
                            Else
                                If Not .AtEndOfStream Then
                                    sTextLine(5) = .ReadLine
                                    iEndofEntry = TestEndOfLogEntry(sTextLine(5))
                                Else
                                    iEndofEntry = 0
                                End If
                                
                                If (iEndofEntry = 0) Then
                                    sTextLine(5) = "No Entry"
                                Else
                                    If Not .AtEndOfStream Then .SkipLine
                                End If
                            End If
                            
                            NoUse = Module1.ConditionTest(sTextLine(1), sTextLine(2), sTextLine(3), sTextLine(4), sTextLine(5))
                        Else
                            sTextLine(2) = .ReadLine
                            If Not .AtEndOfStream Then
                                sTextLine(3) = .ReadLine
                                iEndofEntry = TestEndOfLogEntry(sTextLine(3))
                            Else
                                iEndofEntry = 0
                            End If
                            
                            If (iEndofEntry = 0) Then
                                sTextLine(3) = "No entry"
                            Else
                                If Not .AtEndOfStream Then .SkipLine
                            End If
                            
                            NoUse = Module1.ConditionTest(sTextLine(1), sTextLine(2), sTextLine(3))
                        End If
                    Case 4:
                            sTextLine(1) = .ReadLine
                            sTextLine(2) = .ReadLine
                            sTextLine(3) = .ReadLine
                            sTextLine(4) = .ReadLine
                            If Not .AtEndOfStream Then
                                sTextLine(5) = .ReadLine
                                iEndofEntry = TestEndOfLogEntry(sTextLine(5))
                            Else
                                iEndofEntry = 0
                            End If
                            
                            If (iEndofEntry = 0) Then
                                sTextLine(5) = "No entry"
                            Else
                                If Not .AtEndOfStream Then .SkipLine
                            End If
                            sEvent = "Alarm : "
                            NoUse = Module1.ZoneDataExtraction(sTextLine(2), sTextLine(3), sTextLine(4), sTextLine(5))
                    Case 5:
                            MsgBox ("Found an unkown First Entry : " & sTextLine(0))
                End Select
                
                EmptyRow = WorksheetFunction.CountA(Range("A:A")) + 1
                
                Sheet1.Cells(EmptyRow, 1) = sDate
                Sheet1.Cells(EmptyRow, 2) = sEvent
                Sheet1.Cells(EmptyRow, 3) = sPlace
                
                sDate = ""
                sEvent = ""
                sPlace = ""
            Loop
    
        End With
    End With
    
    Cells.Select
    With Selection
        .HorizontalAlignment = xlCenter
        .VerticalAlignment = xlCenter
        .WrapText = False
        .Orientation = 0
        .AddIndent = False
        .IndentLevel = 0
        .ShrinkToFit = False
        .ReadingOrder = xlContext
        .MergeCells = False
    End With
    Range("A1").Select
Exit Sub
    
    ' Error Handler to Close opened file if a error is found to stop
    ' the system from trying to run with a open file
ErrHandler:
        Close #1
        MsgBox ("Error Occured, File has been Closed")
    GoTo JumptoEnd
    


JumptoEnd:
End Sub
