object NewProjectForm: TNewProjectForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #1057#1086#1079#1076#1072#1085#1080#1077' '#1087#1088#1086#1077#1082#1090#1072
  ClientHeight = 350
  ClientWidth = 460
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RowAddButton: TSpeedButton
    Left = 179
    Top = 112
    Width = 23
    Height = 22
    Hint = #1044#1086#1073#1072#1074#1080#1090#1100' (insert)'
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7E6E6E6DADADAD3D3D3CF
      CFCFCFCFCFD3D3D3DADADAE6E6E6F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1EBEBEBEBEBEBECEBECFAF1F4F9F1F4EBEBEBEBEBEBEBEBEBF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF83
      D2B587D3B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF009E60009E62FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF09
      AB6D0CAA6EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0FB47211B272FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF70D3AD03B77110BA7815BC7B22
      C38323C38417BC7C13BB7A06B87281D9B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFF78DDB710CC871DCE8D21CF8F28D09329D0941DCE8D19CE8C0BCB8581DF
      BCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFFFBFCFFFCFEFFF7FA29
      DB9F25D69AFFFBFFFFFCFDFFFBFCFFFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFBFD2EE4AD29DEA7FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F92C
      EBB824E5B1FFFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFDFD83E9CC7EE7C8FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = RowAddButtonClick
  end
  object RowRemoveButton: TSpeedButton
    Left = 179
    Top = 140
    Width = 23
    Height = 22
    Hint = #1059#1076#1072#1083#1080#1090#1100' (del)'
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7E6E6E6DADADAD3D3D3CF
      CFCFCFCFCFD3D3D3DADADAE6E6E6F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1EBEBEBEBEBEBEBEBEBECECECECECECEBEBEBEBEBEBEBEBEBF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFC6C6E09090C19696C49797C5A0A0C9A3A3CB9C9CC78B8BBFF6F6
      FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8484CD1B1BAE2525B125
      25B12525B02525B02626B11313AAE4E4EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFAAAAE76666E47878E67C7CE77474E66D6DE56D6DE55F5FE3EBEB
      F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = RowRemoveButtonClick
  end
  object RowMoveUpButton: TSpeedButton
    Left = 179
    Top = 168
    Width = 23
    Height = 22
    Hint = #1055#1086#1076#1085#1103#1090#1100' '#1074#1099#1096#1077
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7EAE7E8BFD5CE7DC4AB82
      C4AD82C4AD7CC4AAC9D8D3E9E7E8F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1F3EEF0A8D2C41C9F6F2BA4782BA4781A9E6EBDDAD1F1EDEEF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBBE3D527A67737
      AC8137AC8125A576D1ECE3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFBBE3D52AAB7C39B18539B18527AA7AD1EBE3FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBDE4D72CB1803C
      B6893CB6892AB07FD3ECE4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFBFE6D92FB7853EBC8E3EBC8E2DB684D4EDE5FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCDEAE032BD8A41
      C29341C29330BD89E3F2EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      B8E3D444C69958CBA254CAA043C99946C99A46C99A43C99856CAA157CAA242C5
      97CAE9DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF92DCC440D4A54FD5AB52D6AC52
      D6AC52D6AC52D6AC4FD5AA3FD3A4A6E1CEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF8FDFC859E3BF63E3C163E3C163E3C163E3C157E3BF9DE1CDFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8DE6D16FEFD676
      EED676EED66DEFD699E5D2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFDFE91ECDD85F8EA84F8EA99EBDCFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAF995
      F3EB9CF1E9FFFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F7F5F6F9F6FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = RowMoveUpButtonClick
  end
  object RowMoveDownButton: TSpeedButton
    Left = 179
    Top = 196
    Width = 23
    Height = 22
    Hint = #1054#1087#1091#1089#1090#1080#1090#1100' '#1085#1080#1078#1077
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7E6E6E6DADADAD9D4D6C2
      D1CCC9D2CFD7D4D5DADADAE6E6E6F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1EBEBEBF0EDEEF0ECEE26A2762DA47AF7EFF3EFEDEDEBEBEBF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3BAF8631
      AA7E2FA97D46B38CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFF4CB79130AD7F39B18539B1852EAC7E59BC9AFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF63C3A12FB2823CB68A3C
      B68A3CB68A3CB68A2CB18071C7A9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFF78CCAF29B5823BBB8C3EBC8E3EBC8E3EBC8E3EBC8E3ABB8C27B58187D2
      B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBAE3D650C39A64C9A55BC7A03DC19141
      C29341C2933DC1915EC7A264C9A54EC299C7E8DDFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFD4EFE637C69246CA9A46CA9A35C591E3F3EEFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCBECE13FD09F4E
      D3A64ED3A63ED09ED9F0E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFCEEEE44ADAAE58DCB358DCB349DAADDBF0E9FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD1EFE656E3BB63
      E5C063E5C054E3BADDF1EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFD3F0E762EAC76EEBCA6EEBCA61EAC7DEF1EBFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2F0E863EFCE6E
      EFD16EEFD162EFCEDDF1EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFECF7F4C0F0E4C5F1E6C5F1E6C0F0E4F0F8F5FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = RowMoveDownButtonClick
  end
  object Label1: TLabel
    Left = 24
    Top = 90
    Width = 166
    Height = 16
    Caption = #1053#1072#1080#1084#1077#1085#1086#1074#1072#1085#1080#1103' '#1072#1083#1100#1090#1077#1088#1085#1072#1090#1080#1074
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object ColAddButton: TSpeedButton
    Left = 419
    Top = 112
    Width = 23
    Height = 22
    Hint = #1044#1086#1073#1072#1074#1080#1090#1100' (insert)'
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7E6E6E6DADADAD3D3D3CF
      CFCFCFCFCFD3D3D3DADADAE6E6E6F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1EBEBEBEBEBEBECEBECFAF1F4F9F1F4EBEBEBEBEBEBEBEBEBF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF83
      D2B587D3B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF009E60009E62FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF09
      AB6D0CAA6EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0FB47211B272FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF70D3AD03B77110BA7815BC7B22
      C38323C38417BC7C13BB7A06B87281D9B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFF78DDB710CC871DCE8D21CF8F28D09329D0941DCE8D19CE8C0BCB8581DF
      BCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFFFBFCFFFCFEFFF7FA29
      DB9F25D69AFFFBFFFFFCFDFFFBFCFFFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFBFD2EE4AD29DEA7FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F92C
      EBB824E5B1FFFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFDFD83E9CC7EE7C8FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = ColAddButtonClick
  end
  object ColRemoveButton: TSpeedButton
    Left = 419
    Top = 140
    Width = 23
    Height = 22
    Hint = #1059#1076#1072#1083#1080#1090#1100' (del)'
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7E6E6E6DADADAD3D3D3CF
      CFCFCFCFCFD3D3D3DADADAE6E6E6F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1EBEBEBEBEBEBEBEBEBECECECECECECEBEBEBEBEBEBEBEBEBF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFC6C6E09090C19696C49797C5A0A0C9A3A3CB9C9CC78B8BBFF6F6
      FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8484CD1B1BAE2525B125
      25B12525B02525B02626B11313AAE4E4EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFAAAAE76666E47878E67C7CE77474E66D6DE56D6DE55F5FE3EBEB
      F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = ColRemoveButtonClick
  end
  object ColMoveUpButton: TSpeedButton
    Left = 419
    Top = 168
    Width = 23
    Height = 22
    Hint = #1055#1086#1076#1085#1103#1090#1100' '#1074#1099#1096#1077
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7EAE7E8BFD5CE7DC4AB82
      C4AD82C4AD7CC4AAC9D8D3E9E7E8F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1F3EEF0A8D2C41C9F6F2BA4782BA4781A9E6EBDDAD1F1EDEEF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBBE3D527A67737
      AC8137AC8125A576D1ECE3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFBBE3D52AAB7C39B18539B18527AA7AD1EBE3FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBDE4D72CB1803C
      B6893CB6892AB07FD3ECE4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFBFE6D92FB7853EBC8E3EBC8E2DB684D4EDE5FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCDEAE032BD8A41
      C29341C29330BD89E3F2EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      B8E3D444C69958CBA254CAA043C99946C99A46C99A43C99856CAA157CAA242C5
      97CAE9DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF92DCC440D4A54FD5AB52D6AC52
      D6AC52D6AC52D6AC4FD5AA3FD3A4A6E1CEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFF8FDFC859E3BF63E3C163E3C163E3C163E3C157E3BF9DE1CDFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8DE6D16FEFD676
      EED676EED66DEFD699E5D2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFDFE91ECDD85F8EA84F8EA99EBDCFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAF995
      F3EB9CF1E9FFFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F7F5F6F9F6FFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = ColMoveUpButtonClick
  end
  object ColMoveDownButton: TSpeedButton
    Left = 419
    Top = 196
    Width = 23
    Height = 22
    Hint = #1054#1087#1091#1089#1090#1080#1090#1100' '#1085#1080#1078#1077
    Glyph.Data = {
      36030000424D3603000000000000360000002800000010000000100000000100
      18000000000000030000C40E0000C40E00000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7E6E6E6DADADAD9D4D6C2
      D1CCC9D2CFD7D4D5DADADAE6E6E6F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFF1F1F1EBEBEBF0EDEEF0ECEE26A2762DA47AF7EFF3EFEDEDEBEBEBF1F1
      F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3BAF8631
      AA7E2FA97D46B38CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFF4CB79130AD7F39B18539B1852EAC7E59BC9AFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF63C3A12FB2823CB68A3C
      B68A3CB68A3CB68A2CB18071C7A9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFF78CCAF29B5823BBB8C3EBC8E3EBC8E3EBC8E3EBC8E3ABB8C27B58187D2
      B7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBAE3D650C39A64C9A55BC7A03DC19141
      C29341C2933DC1915EC7A264C9A54EC299C7E8DDFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFD4EFE637C69246CA9A46CA9A35C591E3F3EEFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCBECE13FD09F4E
      D3A64ED3A63ED09ED9F0E8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFCEEEE44ADAAE58DCB358DCB349DAADDBF0E9FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD1EFE656E3BB63
      E5C063E5C054E3BADDF1EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFD3F0E762EAC76EEBCA6EEBCA61EAC7DEF1EBFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD2F0E863EFCE6E
      EFD16EEFD162EFCEDDF1EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFECF7F4C0F0E4C5F1E6C5F1E6C0F0E4F0F8F5FFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
    ParentShowHint = False
    ShowHint = True
    OnClick = ColMoveDownButtonClick
  end
  object Label2: TLabel
    Left = 266
    Top = 87
    Width = 152
    Height = 16
    Caption = #1053#1072#1080#1084#1077#1085#1086#1074#1072#1085#1080#1103' '#1082#1088#1080#1090#1077#1088#1080#1077#1074
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 24
    Top = 24
    Width = 64
    Height = 13
    Caption = #1048#1084#1103' '#1087#1088#1086#1077#1090#1082#1072
  end
  object Label4: TLabel
    Left = 24
    Top = 51
    Width = 77
    Height = 13
    Caption = #1052#1077#1090#1086#1076' '#1088#1072#1089#1095#1105#1090#1072
  end
  object RowNamesListBox: TListBox
    Left = 24
    Top = 112
    Width = 149
    Height = 169
    ItemHeight = 13
    TabOrder = 0
    OnDblClick = RowsListBoxClick
    OnKeyDown = RowNamesListBoxKeyDown
  end
  object ColNamesListBox: TListBox
    Left = 264
    Top = 112
    Width = 149
    Height = 169
    ItemHeight = 13
    TabOrder = 1
    OnDblClick = ColNamesListBoxDblClick
    OnKeyDown = ColNamesListBoxKeyDown
  end
  object CancelButton: TButton
    Left = 144
    Top = 312
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    OnClick = CancelButtonClick
  end
  object NextButton: TButton
    Left = 239
    Top = 312
    Width = 75
    Height = 25
    Caption = #1044#1072#1083#1077#1077
    TabOrder = 3
    OnClick = NextButtonClick
  end
  object ProjectName: TEdit
    Left = 121
    Top = 21
    Width = 193
    Height = 21
    TabOrder = 4
  end
  object MethodComboBox: TComboBox
    Left = 121
    Top = 48
    Width = 193
    Height = 21
    Hint = #1042#1099#1073#1086#1088' '#1084#1077#1090#1086#1076#1072' '#1088#1072#1089#1095#1105#1090#1072
    Style = csDropDownList
    ItemIndex = 0
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    Text = #1052#1077#1090#1086#1076' '#1074#1079#1074#1077#1096#1077#1085#1085#1086#1081' '#1089#1091#1084#1084#1099' '#1084#1077#1089#1090
    OnSelect = MethodComboBoxSelect
    Items.Strings = (
      #1052#1077#1090#1086#1076' '#1074#1079#1074#1077#1096#1077#1085#1085#1086#1081' '#1089#1091#1084#1084#1099' '#1084#1077#1089#1090
      #1052#1077#1090#1086#1076' '#1072#1085#1072#1083#1080#1079#1072' '#1080#1077#1088#1072#1088#1093#1080#1081)
  end
end
