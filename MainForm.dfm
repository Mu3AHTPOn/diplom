object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 478
  ClientWidth = 900
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object InputDataStringGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 705
    Height = 329
    BevelWidth = 5
    DefaultColWidth = 24
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing, goEditing, goTabs, goFixedColClick, goFixedRowClick]
    TabOrder = 0
    OnKeyDown = InputDataStringGridKeyDown
    OnSelectCell = InputDataStringGridSelectCell
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
    end
    object N5: TMenuItem
      Caption = #1042#1080#1076
      object N12: TMenuItem
        Caption = #1055#1091#1085#1082#1090#1099' '#1085#1072#1089#1090#1088#1086#1077#1082' '#1087#1072#1085#1077#1083#1077#1081' '#1091#1087#1088#1072#1074#1083#1077#1085#1080#1103
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
