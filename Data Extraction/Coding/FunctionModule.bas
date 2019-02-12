Attribute VB_Name = "Module1"
'------------------------------------------'
' Created and Compiled by CORDRE SMITH     '
'           on 19 January 2017             '
' If you have any queries or Errors you    '
' can contact me at smithcordre@gmail.com  '
'------------------------------------------'

Option Compare Text

Function TestOccurance(TextLine) As Integer
    If (InStr(TextLine, "Action") <> 0) Then
        TestOccurance = 1
    ElseIf (InStr(TextLine, "Fault") <> 0) Then
        TestOccurance = 2
    ElseIf (InStr(TextLine, "Condition") <> 0) Then
        TestOccurance = 3
    ElseIf (InStr(TextLine, "Alarm") <> 0) Then
        TestOccurance = 4
    Else
        TestOccurance = 5
    End If
End Function

Function ActionTest(ByVal TextLine2 As String, ByVal TextLine3 As String, ByVal TextLine4 As String, Optional ByVal TextLine5 As String, Optional ByVal TextLine6 As String) As Integer
    
    If (Mid(TextLine2, 1, 1) = " ") Then
        Sheet1.sEvent = "Action" & " : " & WhiteSpaceRemove(TextLine2, False, False)
        Sheet1.sDate = WhiteSpaceRemove(TextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(TextLine4, False, False)
    ElseIf (InStr(TextLine2, "Access") <> 0) Then
        If (InStr(TextLine2, "Enabled") <> 0) Then
            Sheet1.sEvent = "Action" & " : " & "Access Enabled"
        ElseIf (InStr(TextLine2, "Disabled") <> 0) Then
            Sheet1.sEvent = "Action" & " : " & "Access Disabled"
        End If
        Sheet1.sDate = WhiteSpaceRemove(TextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(TextLine4, False, False)
    ElseIf (InStr(TextLine2, "Output") <> 0) Then
        If (InStr(TextLine2, "True") <> 0) Then
            Sheet1.sEvent = "Action" & " : " & "Output (True)"
        ElseIf (InStr(TextLine2, "False") <> 0) Then
            Sheet1.sEvent = "Action" & " : " & "Output (False)"
        End If
        Sheet1.sDate = WhiteSpaceRemove(TextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(TextLine4, False, False)
    ElseIf (InStr(TextLine2, "Input") <> 0) Then
        If (InStr(TextLine2, "True") <> 0) Then
            Sheet1.sEvent = "Action" & " : " & "Input (True)"
        ElseIf (InStr(TextLine2, "False") <> 0) Then
            Sheet1.sEvent = "Action" & " : " & "Input (False)"
        End If
        Sheet1.sDate = WhiteSpaceRemove(TextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(TextLine4, False, False)
    ElseIf (InStr(TextLine2, "G-Rep") <> 0) Then
        Sheet1.sEvent = "Action" & " : " & FindGRep(TextLine2)
        Sheet1.sDate = WhiteSpaceRemove(TextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(TextLine4, False, False)
    ElseIf (InStr(TextLine2, "Zone") <> 0) Then
        Sheet1.sEvent = "Action : "
        If (TextLine5 = "") Then
            NoUse = ZoneDataExtraction(TextLine2, TextLine3, TextLine4, "No Data")
        Else
            NoUse = ZoneDataExtraction(TextLine3, TextLine4, TextLine5, TextLine6)
        End If
    Else
        MsgBox ("Action TEst found an Unkown Word : " & TextLine2)
    End If
    
End Function

Function FaultTest(ByVal sTextLine2 As String, ByVal sTextLine3 As String, ByVal sTextLine4 As String, Optional ByVal sTextLine5 As String, Optional ByVal sTextLine6 As String) As Integer
    If (Mid(sTextLine2, 1, 1) = " ") Then
        Sheet1.sEvent = "Fault" & " : " & WhiteSpaceRemove(sTextLine2, False, False)
        Sheet1.sDate = WhiteSpaceRemove(sTextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(sTextLine4, False, False)
    ElseIf (InStr(sTextLine2, "Zone") <> 0) Then
        Sheet1.sEvent = "Fault : "
        NoUse = ZoneDataExtraction(sTextLine3, sTextLine4, sTextLine5, sTextLine6)
    End If
End Function
Function ConditionTest(ByVal sTextLine2 As String, ByVal sTextLine3 As String, ByVal sTextLine4 As String, Optional ByVal sTextLine5 As String, Optional ByVal sTextLine6 As String) As Integer
    If (InStr(sTextLine2, "Output") <> 0) Then
        If (InStr(sTextLine2, "True") <> 0) Then
            Sheet1.sEvent = "Condition" & " : " & "Output (True)"
        ElseIf (InStr(sTextLine2, "False") <> 0) Then
            Sheet1.sEvent = "Condition" & " : " & "Output (False)"
        End If
        Sheet1.sDate = WhiteSpaceRemove(sTextLine3, False, True)
        Sheet1.sPlace = WhiteSpaceRemove(sTextLine4, False, False)
    ElseIf (InStr(sTextLine2, "Zone") <> 0) Then
        Sheet1.sEvent = "Condition : "
        NoUse = ZoneDataExtraction(sTextLine3, sTextLine4, sTextLine5, sTextLine6)
    End If
End Function
Function FindGRep(ByVal sTextLine As String) As String
    Dim iPos1 As Integer, iPos2 As Integer
    
    iPos1 = InStr(sTextLine, "G-Repeater")
    
    For i = 1 To Len(sTextLine)
        If (Mid(TextLine, Len(sTextLine) - i + 1, 1) <> " ") Then
            iPos2 = Len(sTextLine) - i + 1
            Exit For
        End If
    Next i
    
    FindGRep = Mid(sTextLine, iPos1, iPos2 - iPos1 + 1)
End Function

Function WhiteSpaceRemove(ByVal sText As String, bStartingGibb As Boolean, bDate As Boolean) As String
    Dim iPos1 As Integer, iPos2 As Integer
    Dim bFoundS As Boolean, bFoundE As Boolean

    bFoundE = False
    bFoundS = False
    
    If (bStartingGibb = True) Then
        For i = 1 To Len(sText)
            If (Mid(sText, i, 1) = " ") Then
                iPos1 = i
                Exit For
            End If
        Next i
        
        sText = Mid(sText, iPos1, Len(sText) - iPos1 + 1)
    End If
    
    For i = 1 To Len(sText)
        If ((Mid(sText, i, 1) <> " ") And (bFoundS = False)) Then
            iPos1 = i
            bFoundS = True
        End If
        If ((Mid(sText, Len(sText) - i + 1, 1) <> " ") And (bFoundE = False)) Then
            iPos2 = Len(sText) - i + 1
            bFoundE = True
        End If
    Next i
    If (bDate = True) Then
        WhiteSpaceRemove = Mid(sText, iPos1, iPos2 - iPos1 + 1 - 4)
    Else
        WhiteSpaceRemove = Mid(sText, iPos1, iPos2 - iPos1 + 1)
    End If
End Function

Function ZoneDataExtraction(sTextLine3 As String, sTextLine4 As String, sTextLine5 As String, sTextLine6 As String) As Integer
    Dim iPos1 As Integer, iPos2 As Integer, sTemp As String
    
    For i = 1 To Len(sTextLine3)
        If (Mid(sTextLine3, Len(sTextLine3) - i + 1, 1) <> " ") Then
            iPos1 = Len(sTextLine3) - i + 1
            Exit For
        End If
    Next i
    
    sTemp = Mid(sTextLine3, 1, iPos1)
    
    For i = 1 To Len(sTemp)
        If (Mid(sTemp, Len(sTemp) - i + 1, 1) = " ") Then
            iPos1 = Len(sTemp) - i + 1
            Exit For
        End If
    Next i
    
    Sheet1.sEvent = Sheet1.sEvent & Mid(sTemp, iPos1 + 1, Len(sTemp) - iPos1)
    Sheet1.sDate = WhiteSpaceRemove(sTextLine4, True, True)
    Sheet1.sPlace = WhiteSpaceRemove(sTextLine5, False, False)
    Sheet1.sEvent = Sheet1.sEvent & " : " & WhiteSpaceRemove(sTextLine6, False, False)
    
    ZoneDataExtraction = 1
End Function

Function TestEndOfLogEntry(sTextLine As String) As Integer
    Dim bFoundSomething As Boolean
    bFoundSomething = False
    
    For i = 1 To Len(sTextLine)
        If (Mid(sTextLine, i, 1) <> " " And Mid(sTextLine, i, 1) <> "-") Then
            bFoundSomething = True
            Exit For
        End If
    Next i
    
    If (bFoundSomething = True) Then
        TestEndOfLogEntry = 1
    Else
        TestEndOfLogEntry = 0
    End If
    
End Function

