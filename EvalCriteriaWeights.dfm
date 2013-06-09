object EvalCriteriaWeightsForm: TEvalCriteriaWeightsForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #1042#1099#1095#1080#1089#1083#1077#1085#1080#1077' '#1074#1077#1089#1086#1074#1099#1093' '#1082#1086#1101#1092#1092#1080#1094#1080#1077#1085#1090#1086#1074
  ClientHeight = 484
  ClientWidth = 698
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 177
    Top = 32
    Width = 234
    Height = 13
    Caption = #1047#1072#1076#1072#1081#1090#1077' '#1086#1090#1085#1086#1089#1080#1090#1077#1083#1100#1085#1091#1102' '#1074#1072#1078#1085#1086#1089#1090#1100' '#1082#1088#1080#1090#1077#1088#1080#1077#1074
  end
  object StepLabel: TLabel
    Left = 272
    Top = 13
    Width = 30
    Height = 13
    Caption = #1064#1072#1075' 1'
  end
  object ConsistencLabel: TLabel
    Left = 144
    Top = 389
    Width = 4
    Height = 13
    Caption = '-'
  end
  object Label2: TLabel
    Left = 8
    Top = 389
    Width = 130
    Height = 13
    Caption = #1048#1085#1076#1077#1082#1089' '#1089#1086#1075#1083#1072#1089#1086#1074#1072#1085#1085#1086#1089#1090#1080':'
  end
  object CriteriaEstimates: TStringGrid
    Left = 361
    Top = 72
    Width = 328
    Height = 289
    ColCount = 2
    DefaultColWidth = 24
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 0
    OnDrawCell = CriteriaEstimatesDrawCell
    OnGetEditText = CriteriaEstimatesGetEditText
    OnSetEditText = CriteriaEstimatesSetEditText
  end
  object BackButton: TButton
    Left = 249
    Top = 423
    Width = 75
    Height = 25
    Caption = #1053#1072#1079#1072#1076
    TabOrder = 1
    OnClick = BackButtonClick
  end
  object NextButton: TButton
    Left = 361
    Top = 423
    Width = 75
    Height = 25
    Caption = #1044#1072#1083#1077#1077
    TabOrder = 2
    OnClick = NextButtonClick
  end
  object PairWiseGrid: TStringGrid
    Left = 8
    Top = 64
    Width = 316
    Height = 297
    ColCount = 2
    DefaultColWidth = 24
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goTabs]
    TabOrder = 3
    OnDrawCell = CriteriaEstimatesDrawCell
    OnSetEditText = CriteriaEstimatesSetEditText
  end
end
