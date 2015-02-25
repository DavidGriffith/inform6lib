!-------------------------------------------------------------------------------
!  infglk.h - an Inform library to allow easy access to glk functions
!    under glulx
!  Dynamically created by glk2inf.pl on 1000802 at 11:17.34.
!  Send comments or suggestions to: katre@ruf.rice.edu
!-------------------------------------------------------------------------------
System_file;
#Ifdef TARGET_GLULX;
#Ifndef infglk_h;
Constant infglk_h;
Message "[Including <infglk>]";


Constant GLK_NULL 0;

! Constant definitions from glk.h
Constant gestalt_Version 0;
Constant gestalt_CharInput 1;
Constant gestalt_LineInput 2;
Constant gestalt_CharOutput 3;
Constant gestalt_CharOutput_CannotPrint 0;
Constant gestalt_CharOutput_ApproxPrint 1;
Constant gestalt_CharOutput_ExactPrint 2;
Constant gestalt_MouseInput 4;
Constant gestalt_Timer 5;
Constant gestalt_Graphics 6;
Constant gestalt_DrawImage 7;
Constant gestalt_Sound 8;
Constant gestalt_SoundVolume 9;
Constant gestalt_SoundNotify 10;
Constant gestalt_Hyperlinks 11;
Constant gestalt_HyperlinkInput 12;
Constant gestalt_SoundMusic 13;
Constant gestalt_GraphicsTransparency 14;
Constant evtype_None 0;
Constant evtype_Timer 1;
Constant evtype_CharInput 2;
Constant evtype_LineInput 3;
Constant evtype_MouseInput 4;
Constant evtype_Arrange 5;
Constant evtype_Redraw 6;
Constant evtype_SoundNotify 7;
Constant evtype_Hyperlink 8;
Constant keycode_Unknown $ffffffff;
Constant keycode_Left $fffffffe;
Constant keycode_Right $fffffffd;
Constant keycode_Up $fffffffc;
Constant keycode_Down $fffffffb;
Constant keycode_Return $fffffffa;
Constant keycode_Delete $fffffff9;
Constant keycode_Escape $fffffff8;
Constant keycode_Tab $fffffff7;
Constant keycode_PageUp $fffffff6;
Constant keycode_PageDown $fffffff5;
Constant keycode_Home $fffffff4;
Constant keycode_End $fffffff3;
Constant keycode_Func1 $ffffffef;
Constant keycode_Func2 $ffffffee;
Constant keycode_Func3 $ffffffed;
Constant keycode_Func4 $ffffffec;
Constant keycode_Func5 $ffffffeb;
Constant keycode_Func6 $ffffffea;
Constant keycode_Func7 $ffffffe9;
Constant keycode_Func8 $ffffffe8;
Constant keycode_Func9 $ffffffe7;
Constant keycode_Func10 $ffffffe6;
Constant keycode_Func11 $ffffffe5;
Constant keycode_Func12 $ffffffe4;
Constant keycode_MAXVAL 28;
Constant style_Normal 0;
Constant style_Emphasized 1;
Constant style_Preformatted 2;
Constant style_Header 3;
Constant style_Subheader 4;
Constant style_Alert 5;
Constant style_Note 6;
Constant style_BlockQuote 7;
Constant style_Input 8;
Constant style_User1 9;
Constant style_User2 10;
Constant style_NUMSTYLES 11;
Constant wintype_AllTypes 0;
Constant wintype_Pair 1;
Constant wintype_Blank 2;
Constant wintype_TextBuffer 3;
Constant wintype_TextGrid 4;
Constant wintype_Graphics 5;
Constant winmethod_Left $00;
Constant winmethod_Right $01;
Constant winmethod_Above $02;
Constant winmethod_Below $03;
Constant winmethod_DirMask $0f;
Constant winmethod_Fixed $10;
Constant winmethod_Proportional $20;
Constant winmethod_DivisionMask $f0;
Constant fileusage_Data $00;
Constant fileusage_SavedGame $01;
Constant fileusage_Transcript $02;
Constant fileusage_InputRecord $03;
Constant fileusage_TypeMask $0f;
Constant fileusage_TextMode $100;
Constant fileusage_BinaryMode $000;
Constant filemode_Write $01;
Constant filemode_Read $02;
Constant filemode_ReadWrite $03;
Constant filemode_WriteAppend $05;
Constant seekmode_Start 0;
Constant seekmode_Current 1;
Constant seekmode_End 2;
Constant stylehint_Indentation 0;
Constant stylehint_ParaIndentation 1;
Constant stylehint_Justification 2;
Constant stylehint_Size 3;
Constant stylehint_Weight 4;
Constant stylehint_Oblique 5;
Constant stylehint_Proportional 6;
Constant stylehint_TextColor 7;
Constant stylehint_BackColor 8;
Constant stylehint_ReverseColor 9;
Constant stylehint_NUMHINTS 10;
Constant stylehint_just_LeftFlush 0;
Constant stylehint_just_LeftRight 1;
Constant stylehint_just_Centered 2;
Constant stylehint_just_RightFlush 3;
Constant imagealign_InlineUp $01;
Constant imagealign_InlineDown $02;
Constant imagealign_InlineCenter $03;
Constant imagealign_MarginLeft $04;
Constant imagealign_MarginRight $05;

! The actual glk functions.
[ glk_exit 
      ret;
  ! Must push arguments onto the stack in reverse order
  ! And now the @glk call
@glk 1 0 ret;
  return ret;
];

[ glk_set_interrupt_handler func
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy func sp;
  ! And now the @glk call
@glk 2 1 ret;
  return ret;
];

[ glk_tick 
      ret;
  ! Must push arguments onto the stack in reverse order
  ! And now the @glk call
@glk 3 0 ret;
  return ret;
];

[ glk_gestalt sel val
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy val sp;
  @copy sel sp;
  ! And now the @glk call
@glk 4 2 ret;
  return ret;
];

[ glk_gestalt_ext sel val arr arrlen
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy arrlen sp;
  @copy arr sp;
  @copy val sp;
  @copy sel sp;
  ! And now the @glk call
@glk 5 4 ret;
  return ret;
];

[ glk_char_to_lower ch
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy ch sp;
  ! And now the @glk call
@glk 160 1 ret;
  return ret;
];

[ glk_char_to_upper ch
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy ch sp;
  ! And now the @glk call
@glk 161 1 ret;
  return ret;
];

[ glk_window_get_root 
      ret;
  ! Must push arguments onto the stack in reverse order
  ! And now the @glk call
@glk 34 0 ret;
  return ret;
];

[ glk_window_open split method size wintype rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy wintype sp;
  @copy size sp;
  @copy method sp;
  @copy split sp;
  ! And now the @glk call
@glk 35 5 ret;
  return ret;
];

[ glk_window_close win result
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy result sp;
  @copy win sp;
  ! And now the @glk call
@glk 36 2 ret;
  return ret;
];

[ glk_window_get_size win widthptr heightptr
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy heightptr sp;
  @copy widthptr sp;
  @copy win sp;
  ! And now the @glk call
@glk 37 3 ret;
  return ret;
];

[ glk_window_set_arrangement win method size keywin
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy keywin sp;
  @copy size sp;
  @copy method sp;
  @copy win sp;
  ! And now the @glk call
@glk 38 4 ret;
  return ret;
];

[ glk_window_get_arrangement win methodptr sizeptr keywinptr
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy keywinptr sp;
  @copy sizeptr sp;
  @copy methodptr sp;
  @copy win sp;
  ! And now the @glk call
@glk 39 4 ret;
  return ret;
];

[ glk_window_iterate win rockptr
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rockptr sp;
  @copy win sp;
  ! And now the @glk call
@glk 32 2 ret;
  return ret;
];

[ glk_window_get_rock win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 33 1 ret;
  return ret;
];

[ glk_window_get_type win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 40 1 ret;
  return ret;
];

[ glk_window_get_parent win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 41 1 ret;
  return ret;
];

[ glk_window_get_sibling win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 48 1 ret;
  return ret;
];

[ glk_window_clear win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 42 1 ret;
  return ret;
];

[ glk_window_move_cursor win xpos ypos
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy ypos sp;
  @copy xpos sp;
  @copy win sp;
  ! And now the @glk call
@glk 43 3 ret;
  return ret;
];

[ glk_window_get_stream win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 44 1 ret;
  return ret;
];

[ glk_window_set_echo_stream win str
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy str sp;
  @copy win sp;
  ! And now the @glk call
@glk 45 2 ret;
  return ret;
];

[ glk_window_get_echo_stream win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 46 1 ret;
  return ret;
];

[ glk_set_window win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 47 1 ret;
  return ret;
];

[ glk_stream_open_file fileref fmode rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy fmode sp;
  @copy fileref sp;
  ! And now the @glk call
@glk 66 3 ret;
  return ret;
];

[ glk_stream_open_memory buf buflen fmode rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy fmode sp;
  @copy buflen sp;
  @copy buf sp;
  ! And now the @glk call
@glk 67 4 ret;
  return ret;
];

[ glk_stream_close str result
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy result sp;
  @copy str sp;
  ! And now the @glk call
@glk 68 2 ret;
  return ret;
];

[ glk_stream_iterate str rockptr
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rockptr sp;
  @copy str sp;
  ! And now the @glk call
@glk 64 2 ret;
  return ret;
];

[ glk_stream_get_rock str
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy str sp;
  ! And now the @glk call
@glk 65 1 ret;
  return ret;
];

[ glk_stream_set_position str pos seekmode
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy seekmode sp;
  @copy pos sp;
  @copy str sp;
  ! And now the @glk call
@glk 69 3 ret;
  return ret;
];

[ glk_stream_get_position str
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy str sp;
  ! And now the @glk call
@glk 70 1 ret;
  return ret;
];

[ glk_stream_set_current str
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy str sp;
  ! And now the @glk call
@glk 71 1 ret;
  return ret;
];

[ glk_stream_get_current 
      ret;
  ! Must push arguments onto the stack in reverse order
  ! And now the @glk call
@glk 72 0 ret;
  return ret;
];

[ glk_put_char ch
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy ch sp;
  ! And now the @glk call
@glk 128 1 ret;
  return ret;
];

[ glk_put_char_stream str ch
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy ch sp;
  @copy str sp;
  ! And now the @glk call
@glk 129 2 ret;
  return ret;
];

[ glk_put_string s
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy s sp;
  ! And now the @glk call
@glk 130 1 ret;
  return ret;
];

[ glk_put_string_stream str s
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy s sp;
  @copy str sp;
  ! And now the @glk call
@glk 131 2 ret;
  return ret;
];

[ glk_put_buffer buf len
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy len sp;
  @copy buf sp;
  ! And now the @glk call
@glk 132 2 ret;
  return ret;
];

[ glk_put_buffer_stream str buf len
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy len sp;
  @copy buf sp;
  @copy str sp;
  ! And now the @glk call
@glk 133 3 ret;
  return ret;
];

[ glk_set_style styl
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy styl sp;
  ! And now the @glk call
@glk 134 1 ret;
  return ret;
];

[ glk_set_style_stream str styl
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy styl sp;
  @copy str sp;
  ! And now the @glk call
@glk 135 2 ret;
  return ret;
];

[ glk_get_char_stream str
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy str sp;
  ! And now the @glk call
@glk 144 1 ret;
  return ret;
];

[ glk_get_line_stream str buf len
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy len sp;
  @copy buf sp;
  @copy str sp;
  ! And now the @glk call
@glk 145 3 ret;
  return ret;
];

[ glk_get_buffer_stream str buf len
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy len sp;
  @copy buf sp;
  @copy str sp;
  ! And now the @glk call
@glk 146 3 ret;
  return ret;
];

[ glk_stylehint_set wintype styl hint val
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy val sp;
  @copy hint sp;
  @copy styl sp;
  @copy wintype sp;
  ! And now the @glk call
@glk 176 4 ret;
  return ret;
];

[ glk_stylehint_clear wintype styl hint
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy hint sp;
  @copy styl sp;
  @copy wintype sp;
  ! And now the @glk call
@glk 177 3 ret;
  return ret;
];

[ glk_style_distinguish win styl1 styl2
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy styl2 sp;
  @copy styl1 sp;
  @copy win sp;
  ! And now the @glk call
@glk 178 3 ret;
  return ret;
];

[ glk_style_measure win styl hint result
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy result sp;
  @copy hint sp;
  @copy styl sp;
  @copy win sp;
  ! And now the @glk call
@glk 179 4 ret;
  return ret;
];

[ glk_fileref_create_temp usage rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy usage sp;
  ! And now the @glk call
@glk 96 2 ret;
  return ret;
];

[ glk_fileref_create_by_name usage name rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy name sp;
  @copy usage sp;
  ! And now the @glk call
@glk 97 3 ret;
  return ret;
];

[ glk_fileref_create_by_prompt usage fmode rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy fmode sp;
  @copy usage sp;
  ! And now the @glk call
@glk 98 3 ret;
  return ret;
];

[ glk_fileref_create_from_fileref usage fref rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  @copy fref sp;
  @copy usage sp;
  ! And now the @glk call
@glk 104 3 ret;
  return ret;
];

[ glk_fileref_destroy fref
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy fref sp;
  ! And now the @glk call
@glk 99 1 ret;
  return ret;
];

[ glk_fileref_iterate fref rockptr
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rockptr sp;
  @copy fref sp;
  ! And now the @glk call
@glk 100 2 ret;
  return ret;
];

[ glk_fileref_get_rock fref
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy fref sp;
  ! And now the @glk call
@glk 101 1 ret;
  return ret;
];

[ glk_fileref_delete_file fref
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy fref sp;
  ! And now the @glk call
@glk 102 1 ret;
  return ret;
];

[ glk_fileref_does_file_exist fref
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy fref sp;
  ! And now the @glk call
@glk 103 1 ret;
  return ret;
];

[ glk_select event
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy event sp;
  ! And now the @glk call
@glk 192 1 ret;
  return ret;
];

[ glk_select_poll event
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy event sp;
  ! And now the @glk call
@glk 193 1 ret;
  return ret;
];

[ glk_request_timer_events millisecs
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy millisecs sp;
  ! And now the @glk call
@glk 214 1 ret;
  return ret;
];

[ glk_request_line_event win buf maxlen initlen
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy initlen sp;
  @copy maxlen sp;
  @copy buf sp;
  @copy win sp;
  ! And now the @glk call
@glk 208 4 ret;
  return ret;
];

[ glk_request_char_event win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 210 1 ret;
  return ret;
];

[ glk_request_mouse_event win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 212 1 ret;
  return ret;
];

[ glk_cancel_line_event win event
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy event sp;
  @copy win sp;
  ! And now the @glk call
@glk 209 2 ret;
  return ret;
];

[ glk_cancel_char_event win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 211 1 ret;
  return ret;
];

[ glk_cancel_mouse_event win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 213 1 ret;
  return ret;
];

[ glk_image_draw win image val1 val2
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy val2 sp;
  @copy val1 sp;
  @copy image sp;
  @copy win sp;
  ! And now the @glk call
@glk 225 4 ret;
  return ret;
];

[ glk_image_draw_scaled win image val1 val2 width height
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy height sp;
  @copy width sp;
  @copy val2 sp;
  @copy val1 sp;
  @copy image sp;
  @copy win sp;
  ! And now the @glk call
@glk 226 6 ret;
  return ret;
];

[ glk_image_get_info image width height
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy height sp;
  @copy width sp;
  @copy image sp;
  ! And now the @glk call
@glk 224 3 ret;
  return ret;
];

[ glk_window_flow_break win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 232 1 ret;
  return ret;
];

[ glk_window_erase_rect win left top width height
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy height sp;
  @copy width sp;
  @copy top sp;
  @copy left sp;
  @copy win sp;
  ! And now the @glk call
@glk 233 5 ret;
  return ret;
];

[ glk_window_fill_rect win color left top width height
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy height sp;
  @copy width sp;
  @copy top sp;
  @copy left sp;
  @copy color sp;
  @copy win sp;
  ! And now the @glk call
@glk 234 6 ret;
  return ret;
];

[ glk_window_set_background_color win color
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy color sp;
  @copy win sp;
  ! And now the @glk call
@glk 235 2 ret;
  return ret;
];

[ glk_schannel_create rock
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rock sp;
  ! And now the @glk call
@glk 242 1 ret;
  return ret;
];

[ glk_schannel_destroy chan
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy chan sp;
  ! And now the @glk call
@glk 243 1 ret;
  return ret;
];

[ glk_schannel_iterate chan rockptr
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy rockptr sp;
  @copy chan sp;
  ! And now the @glk call
@glk 240 2 ret;
  return ret;
];

[ glk_schannel_get_rock chan
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy chan sp;
  ! And now the @glk call
@glk 241 1 ret;
  return ret;
];

[ glk_schannel_play chan snd
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy snd sp;
  @copy chan sp;
  ! And now the @glk call
@glk 248 2 ret;
  return ret;
];

[ glk_schannel_play_ext chan snd repeats notify
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy notify sp;
  @copy repeats sp;
  @copy snd sp;
  @copy chan sp;
  ! And now the @glk call
@glk 249 4 ret;
  return ret;
];

[ glk_schannel_stop chan
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy chan sp;
  ! And now the @glk call
@glk 250 1 ret;
  return ret;
];

[ glk_schannel_set_volume chan vol
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy vol sp;
  @copy chan sp;
  ! And now the @glk call
@glk 251 2 ret;
  return ret;
];

[ glk_sound_load_hint snd flag
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy flag sp;
  @copy snd sp;
  ! And now the @glk call
@glk 252 2 ret;
  return ret;
];

[ glk_set_hyperlink linkval
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy linkval sp;
  ! And now the @glk call
@glk 256 1 ret;
  return ret;
];

[ glk_set_hyperlink_stream str linkval
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy linkval sp;
  @copy str sp;
  ! And now the @glk call
@glk 257 2 ret;
  return ret;
];

[ glk_request_hyperlink_event win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 258 1 ret;
  return ret;
];

[ glk_cancel_hyperlink_event win
      ret;
  ! Must push arguments onto the stack in reverse order
  @copy win sp;
  ! And now the @glk call
@glk 259 1 ret;
  return ret;
];

#Endif;
#Endif;
