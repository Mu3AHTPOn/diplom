object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 541
  ClientWidth = 1058
  Color = clBtnFace
  Constraints.MinHeight = 600
  Constraints.MinWidth = 900
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    1058
    541)
  PixelsPerInch = 96
  TextHeight = 13
  object InputDataStringGrid: TStringGrid
    Left = 0
    Top = 24
    Width = 817
    Height = 367
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelWidth = 5
    DefaultColWidth = 24
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing, goEditing, goTabs, goFixedColClick, goFixedRowClick]
    TabOrder = 0
    OnKeyDown = InputDataStringGridKeyDown
    OnSelectCell = InputDataStringGridSelectCell
  end
  object ToolBar2: TToolBar
    Left = 0
    Top = 0
    Width = 1058
    Height = 29
    Caption = 'ToolBar2'
    TabOrder = 1
    object SpeedButton4: TSpeedButton
      Left = 0
      Top = 0
      Width = 23
      Height = 22
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000C40E0000C40E00000000000000000000F8F8F8C6C6C6
        C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5CACACAFAFAFAFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC6C6C6F7F7F6F9F9F9FBFBFBFBFBFBFCFCFCFDFDFDFF
        FFFFEAE9E99D9D9AF5F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5C5C5F7F7F6
        F8F8F8FAFAFAFBFBFBFCFCFCFDFDFDFEFEFEF4F4F4CCCCCC979793EDEDEDFFFF
        FFFFFFFFFFFFFFFFFFFFC5C5C5F6F6F6F8F8F8FAFAFAFBFBFBFCFCFCFDFDFDFE
        FEFEFBFBFBC4C4C4FDFDFD999995F5F5F5FFFFFFFFFFFFFFFFFFC5C5C5F6F6F5
        F7F7F7FAFAFAFBFBFBFCFCFCFDFDFDFEFEFEFEFEFEE0E0E0C6C6C6CFCFCEA2A1
        9FFAFAFAFFFFFFFFFFFFC5C5C5F5F5F4F6F6F6F9F9F9FAFAFAFBFBFBFCFCFCFD
        FDFDFEFEFEFEFEFEFCFCFCFAFAFAF0EFEFCACACAFFFFFFFFFFFFC5C5C5F4F4F4
        F5F5F5F7F7F7FAFAFAFAFAFAFBFBFBFCFCFCFDFDFDFDFDFDFEFEFEFEFEFEFEFE
        FEC5C5C5FFFFFFFFFFFFC5C5C5F4F4F3F4F4F3F6F6F6F8F8F8FAFAFAFBFBFBFB
        FBFBFCFCFCFCFCFCFDFDFDFDFDFDFDFDFDC5C5C5FFFFFFFFFFFFC5C5C5F2F2F2
        F2F2F2F4F4F4F6F6F6F8F8F8FAFAFAFAFAFAFBFBFBFBFBFBFCFCFCFCFCFCFCFC
        FCC5C5C5FFFFFFFFFFFFC5C5C5F1F1F0F1F1F0F3F3F2F5F5F4F6F6F6F8F8F8F9
        F9F9FAFAFAFAFAFA95DF9523C4230AAF0A1CBB1C99E399FFFFFFC5C5C5EFEFEE
        EFEFEEF1F1F0F3F3F2F4F4F4F6F6F6F7F7F7F8F8F895DD9502AD0200A31AFFFF
        FF00B03602AE0599E099C5C5C5EEEEEDEDEDEDEFEFEEF1F1F0F2F2F2F4F4F3F5
        F5F5F6F6F621B42100A00300A034FFFFFF00B06400AF3223B423C5C5C5ECECEB
        ECECEBEDEDECEFEFEEF0F0EFF1F1F1F2F2F2F3F3F3099509FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFF099709C5C5C5EAE9E8E9E9E8EBEBEAEDEDECEEEEEDEFEFEEF0
        F0EFF1F1F021A6211DB61D29B43FFFFFFF29BC5A1DBC3323A923C5C5C5EAEAEA
        E9E9E8EBEBEAEDEDECEEEEEDEFEFEEF0F0EFF1F1F08FCF8F36BF3678D578FFFF
        FF78D57A37C03798D798EFEFEFC5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5
        C5C5C5C5C5C5C5C577B5773DB03D5FC45F42B44297D897FFFFFF}
    end
    object SpeedButton3: TSpeedButton
      Left = 23
      Top = 0
      Width = 23
      Height = 22
    end
    object SpeedButton2: TSpeedButton
      Left = 46
      Top = 0
      Width = 23
      Height = 22
    end
    object SpeedButton1: TSpeedButton
      Left = 69
      Top = 0
      Width = 23
      Height = 22
    end
  end
  object Params: TValueListEditor
    Left = 815
    Top = 24
    Width = 242
    Height = 367
    Anchors = [akTop, akRight, akBottom]
    DisplayOptions = [doAutoColResize, doKeyColFixed]
    Strings.Strings = (
      #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1082#1088#1080#1090#1077#1088#1080#1077#1074'=5'
      #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1086#1073#1098#1077#1082#1090#1086#1074'=8')
    TabOrder = 2
    OnValidate = ParamsValidate
    ColWidths = (
      148
      88)
  end
  object ResultRichEdit: TRichEdit
    Left = 0
    Top = 414
    Width = 1058
    Height = 130
    Anchors = [akLeft, akRight, akBottom]
    Font.Charset = RUSSIAN_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 3
  end
  object Panel1: TPanel
    Left = 0
    Top = 390
    Width = 1058
    Height = 25
    Anchors = [akLeft, akRight, akBottom]
    Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090#1099
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
  end
  object MainMenu1: TMainMenu
    Left = 736
    Top = 360
    object N1: TMenuItem
      Caption = #1060#1072#1081#1083
      object N2: TMenuItem
        Caption = #1053#1086#1074#1099#1081' '#1087#1088#1086#1077#1082#1090
      end
      object N3: TMenuItem
        Caption = #1054#1090#1082#1088#1099#1090#1100' '#1087#1088#1086#1077#1082#1090
      end
      object N4: TMenuItem
        Caption = #1047#1072#1082#1088#1099#1090#1100' '#1087#1088#1086#1077#1082#1090
      end
      object N14: TMenuItem
        Caption = #1047#1072#1082#1088#1099#1090#1100' '#1087#1088#1086#1075#1088#1072#1084#1084#1091
        OnClick = N14Click
      end
    end
    object N6: TMenuItem
      Caption = #1056#1072#1089#1095#1105#1090
      object N7: TMenuItem
        Caption = #1052#1077#1090#1086#1076' '#1088#1072#1089#1095#1105#1090#1072
      end
      object N8: TMenuItem
        Caption = #1056#1072#1089#1095#1105#1090
        OnClick = N8Click
      end
    end
    object N11: TMenuItem
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099
      object N13: TMenuItem
        Caption = #1055#1091#1085#1082#1090#1099' '#1085#1072#1089#1090#1088#1086#1077#1082' '#1087#1072#1088#1072#1084#1077#1090#1088#1086#1074' '#1084#1077#1090#1086#1076#1086#1074' '
      end
    end
    object N9: TMenuItem
      Caption = #1055#1086#1084#1086#1097#1100
      object N10: TMenuItem
        Caption = #1057#1087#1088#1072#1074#1082#1072
      end
    end
  end
end
