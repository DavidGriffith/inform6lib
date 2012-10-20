! ==============================================================================
!   IO:  Input-Output Routines.
!
!   Supplied for use with Inform 6 -- Release 6/12 -- Serial number 121019
!
!   Copyright Graham Nelson 1993-2012 but freely usable (see manuals)
!
!   This file is automatically Included in your game file by "Parser".
! ------------------------------------------------------------------------------
!   During development of 6/12 it was noticed that too much stuff was
!   crammed into parserm.h.  V5 modules cannot exceed 65535 bytes, so
!   IO-related routines were moved into this new file.
! ------------------------------------------------------------------------------


System_file;

#Ifdef MODULE_MODE;
Constant DEBUG;
Constant Grammar__Version 2;
Include "linklpa";
Import global just_undone;
Import global undo_flag;
Import global print_anything_result;
Import global saveIndef;
Import global indef_mode;
Import global saveCaps;
Import global caps_mode;
#Endif; ! MODULE_MODE


! ----------------------------------------------------------------------------
!   The Keyboard routine actually receives the player's words,
!   putting the words in "a_buffer" and their dictionary addresses in
!   "a_table".  It is assumed that the table is the same one on each
!   (standard) call.
!
!   It can also be used by miscellaneous routines in the game to ask
!   yes-no questions and the like, without invoking the rest of the parser.
!
!   Return the number of words typed
! ----------------------------------------------------------------------------

#Ifdef TARGET_ZCODE;

[ KeyboardPrimitive  a_buffer a_table;
    read a_buffer a_table;

    #Iftrue (#version_number == 6);
    @output_stream -1;
    @loadb a_buffer 1 -> sp;
    @add a_buffer 2 -> sp;
    @print_table sp sp;
    new_line;
    @output_stream 1;
    #Endif;
];

[ KeyCharPrimitive win  key;
    if (win) @set_window win;
    @read_char 1 -> key;
    return key;
];

[ KeyTimerInterrupt;
    rtrue;
];

[ KeyDelay tenths  key;
    @read_char 1 tenths KeyTimerInterrupt -> key;
    return key;
];

#Ifnot; ! TARGET_GLULX

[ KeyCharPrimitive win nostat done res ix jx ch;
    jx = ch; ! squash compiler warnings
    if (win == 0) win = gg_mainwin;
    if (gg_commandstr ~= 0 && gg_command_reading ~= false) {
        ! get_line_stream
        done = glk($0091, gg_commandstr, gg_arguments, 31);
        if (done == 0) {
            glk($0044, gg_commandstr, 0); ! stream_close
            gg_commandstr = 0;
            gg_command_reading = false;
            ! fall through to normal user input.
        }
        else {
            ! Trim the trailing newline
            if (gg_arguments->(done-1) == 10) done = done-1;
            res = gg_arguments->0;
            if (res == '\') {
                res = 0;
                for (ix=1 : ix<done : ix++) {
                    ch = gg_arguments->ix;
                    if (ch >= '0' && ch <= '9') {
                        @shiftl res 4 res;
                        res = res + (ch-'0');
                    }
                    else if (ch >= 'a' && ch <= 'f') {
                        @shiftl res 4 res;
                        res = res + (ch+10-'a');
                    }
                    else if (ch >= 'A' && ch <= 'F') {
                        @shiftl res 4 res;
                        res = res + (ch+10-'A');
                    }
                }
            }
        jump KCPContinue;
        }
    }
    done = false;
    glk($00D2, win); ! request_char_event
    while (~~done) {
        glk($00C0, gg_event); ! select
        switch (gg_event-->0) {
          5: ! evtype_Arrange
            if (nostat) {
                glk($00D3, win); ! cancel_char_event
                res = $80000000;
                done = true;
                break;
            }
            DrawStatusLine();
          2: ! evtype_CharInput
            if (gg_event-->1 == win) {
                res = gg_event-->2;
                done = true;
                }
        }
        ix = HandleGlkEvent(gg_event, 1, gg_arguments);
        if (ix == 0) ix = LibraryExtensions.RunWhile(ext_handleglkevent, 0, gg_event, 1, gg_arguments);
        if (ix == 2) {
            res = gg_arguments-->0;
            done = true;
        }
        else if (ix == -1) {
            done = false;
        }
    }
    if (gg_commandstr ~= 0 && gg_command_reading == false) {
        if (res < 32 || res >= 256 || (res == '\' or ' ')) {
            glk($0081, gg_commandstr, '\'); ! put_buffer_char
            done = 0;
            jx = res;
            for (ix=0 : ix<8 : ix++) {
                @ushiftr jx 28 ch;
                @shiftl jx 4 jx;
                ch = ch & $0F;
                if (ch ~= 0 || ix == 7) done = 1;
                if (done) {
                    if (ch >= 0 && ch <= 9) ch = ch + '0';
                    else                    ch = (ch - 10) + 'A';
                    glk($0081, gg_commandstr, ch); ! put_buffer_char
                }
            }
        }
        else {
            glk($0081, gg_commandstr, res); ! put_buffer_char
        }
        glk($0081, gg_commandstr, 10); ! put_char_stream (newline)
    }
  .KCPContinue;
    return res;
];

[ KeyDelay tenths  key done ix;
    glk($00D2, gg_mainwin); ! request_char_event
    glk($00D6, tenths*100); ! request_timer_events
    while (~~done) {
        glk($00C0, gg_event); ! select
        ix = HandleGlkEvent(gg_event, 1, gg_arguments);
        if (ix == 0) ix = LibraryExtensions.RunWhile(ext_handleglkevent, 0, gg_event, 1, gg_arguments);
        if (ix == 2) {
            key = gg_arguments-->0;
            done = true;
        }
        else if (ix >= 0 && gg_event-->0 == 1 or 2) {
            key = gg_event-->2;
            done = true;
        }
    }
    glk($00D3, gg_mainwin); ! cancel_char_event
    glk($00D6, 0); ! request_timer_events
    return key;
];

[ KeyboardPrimitive  a_buffer a_table done ix;
    if (gg_commandstr ~= 0 && gg_command_reading ~= false) {
        ! get_line_stream
        done = glk($0091, gg_commandstr, a_buffer+WORDSIZE, (INPUT_BUFFER_LEN-WORDSIZE)-1);
        if (done == 0) {
            glk($0044, gg_commandstr, 0); ! stream_close
            gg_commandstr = 0;
            gg_command_reading = false;
            ! L__M(##CommandsRead, 5); would come after prompt
            ! fall through to normal user input.
        }
        else {
            ! Trim the trailing newline
            if ((a_buffer+WORDSIZE)->(done-1) == 10) done = done-1;
            a_buffer-->0 = done;
            glk($0086, 8); ! set input style
            glk($0084, a_buffer+WORDSIZE, done); ! put_buffer
            glk($0086, 0); ! set normal style
            print "^";
            jump KPContinue;
        }
    }
    done = false;
    glk($00D0, gg_mainwin, a_buffer+WORDSIZE, INPUT_BUFFER_LEN-WORDSIZE, 0); ! request_line_event
    while (~~done) {
        glk($00C0, gg_event); ! select
        switch (gg_event-->0) {
          5: ! evtype_Arrange
            DrawStatusLine();
          3: ! evtype_LineInput
            if (gg_event-->1 == gg_mainwin) {
                a_buffer-->0 = gg_event-->2;
                done = true;
            }
        }
        ix = HandleGlkEvent(gg_event, 0, a_buffer);
        if (ix == 0) ix = LibraryExtensions.RunWhile(ext_handleglkevent, 0, gg_event, 0, a_buffer);
        if (ix == 2) done = true;
        else if (ix == -1) done = false;
    }
    if (gg_commandstr ~= 0 && gg_command_reading == false) {
        ! put_buffer_stream
        glk($0085, gg_commandstr, a_buffer+WORDSIZE, a_buffer-->0);
        glk($0081, gg_commandstr, 10); ! put_char_stream (newline)
    }
  .KPContinue;
    Tokenise__(a_buffer,a_table);
    ! It's time to close any quote window we've got going.
    if (gg_quotewin) {
        glk($0024, gg_quotewin, 0); ! close_window
        gg_quotewin = 0;
    }
];

#Endif; ! TARGET_

[ Keyboard  a_buffer a_table  nw i w w2 x1 x2;
    DisplayStatus();

  .FreshInput;

    ! Save the start of the buffer, in case "oops" needs to restore it
    ! to the previous time's buffer

    for (i=0 : i<OOPS_WORKSPACE_LEN : i++) oops_workspace->i = a_buffer->i;

    ! In case of an array entry corruption that shouldn't happen, but would be
    ! disastrous if it did:

    #Ifdef TARGET_ZCODE;
    a_buffer->0 = INPUT_BUFFER_LEN - WORDSIZE;
    a_table->0  = MAX_BUFFER_WORDS; ! Allow to split input into this many words
    #Endif; ! TARGET_

    ! Print the prompt, and read in the words and dictionary addresses

    L__M(##Prompt);
    if (AfterPrompt() == 0) LibraryExtensions.RunAll(ext_afterprompt);
    #IfV5;
    DrawStatusLine();
    #Endif; ! V5
    KeyboardPrimitive(a_buffer, a_table);
    nw = NumberWords(a_table);

    ! If the line was blank, get a fresh line
    if (nw == 0) {
        L__M(##Miscellany, 10);
        jump FreshInput;
    }

    ! Unless the opening word was "oops", return
    ! Conveniently, a_table-->1 is the first word in both ZCODE and GLULX.

    w = a_table-->1;
    if (w == OOPS1__WD or OOPS2__WD or OOPS3__WD) jump DoOops;

    if (a_buffer->WORDSIZE == COMMENT_CHARACTER) {
        #Ifdef TARGET_ZCODE;
        if ((HDR_GAMEFLAGS-->0) & $0001 || xcommsdir)
                                           L__M(##Miscellany, 54);
        else                               L__M(##Miscellany, 55);
        #Ifnot; ! TARGET_GLULX
        if (gg_scriptstr || gg_commandstr) L__M(##Miscellany, 54);
        else                               L__M(##Miscellany, 55);
        #Endif; ! TARGET_

        jump FreshInput;
    }

    #IfV5;
    ! Undo handling

    if ((w == UNDO1__WD or UNDO2__WD or UNDO3__WD) && (nw==1)) {
        if (turns == START_MOVE) {
            L__M(##Miscellany, 11);
            jump FreshInput;
        }
        if (undo_flag == 0) {
            L__M(##Miscellany, 6);
            jump FreshInput;
        }
        if (undo_flag == 1) jump UndoFailed;
        #Ifdef TARGET_ZCODE;
        @restore_undo i;
        #Ifnot; ! TARGET_GLULX
        @restoreundo i;
        i = (~~i);
        #Endif; ! TARGET_
        if (i == 0) {
          .UndoFailed;
            L__M(##Miscellany, 7);
        }
        jump FreshInput;
    }
    #Ifdef TARGET_ZCODE;
    @save_undo i;
    #Ifnot; ! TARGET_GLULX
    @saveundo i;
    if (i == -1) {
        GGRecoverObjects();
        i = 2;
    }
    else  i = (~~i);
    #Endif; ! TARGET_
    just_undone = 0;
    undo_flag = 2;
    if (i == -1) undo_flag = 0;
    if (i == 0) undo_flag = 1;
    if (i == 2) {
        RestoreColours();
        #Ifdef TARGET_ZCODE;
        style bold;
        #Ifnot; ! TARGET_GLULX
        glk($0086, 4); ! set subheader style
        #Endif; ! TARGET_
        print (name) location, "^";
        #Ifdef TARGET_ZCODE;
        style roman;
        #Ifnot; ! TARGET_GLULX
        glk($0086, 0); ! set normal style
        #Endif; ! TARGET_
        L__M(##Miscellany, 13);
        just_undone = 1;
        jump FreshInput;
    }
    #Endif; ! V5

    return nw;

  .DoOops;
    if (oops_from == 0) {
        L__M(##Miscellany, 14);
        jump FreshInput;
    }
    if (nw == 1) {
        L__M(##Miscellany, 15);
        jump FreshInput;
    }
    if (nw > 2) {
        L__M(##Miscellany, 16);
        jump FreshInput;
    }

    ! So now we know: there was a previous mistake, and the player has
    ! attempted to correct a single word of it.

    for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer2->i = a_buffer->i;
    #Ifdef TARGET_ZCODE;
    x1 = a_table->9;  ! Start of word following "oops"
    x2 = a_table->8;  ! Length of word following "oops"
    #Ifnot; ! TARGET_GLULX
    x1 = a_table-->6; ! Start of word following "oops"
    x2 = a_table-->5; ! Length of word following "oops"
    #Endif; ! TARGET_

    ! Repair the buffer to the text that was in it before the "oops"
    ! was typed:

    for (i=0 : i < OOPS_WORKSPACE_LEN : i++) a_buffer->i = oops_workspace->i;
    Tokenise__(a_buffer, a_table);

    ! Work out the position in the buffer of the word to be corrected:

    #Ifdef TARGET_ZCODE;
    w = a_table->(4*oops_from + 1); ! Start of word to go
    w2 = a_table->(4*oops_from);    ! Length of word to go

    #Ifnot; ! TARGET_GLULX
    w = a_table-->(3*oops_from);      ! Start of word to go
    w2 = a_table-->(3*oops_from - 1); ! Length of word to go
    #Endif; ! TARGET_

    ! Write spaces over the word to be corrected:

    for (i=0 : i<w2 : i++) a_buffer->(i+w) = ' ';

    if (w2 < x2) {
        ! If the replacement is longer than the original, move up...
        for (i=INPUT_BUFFER_LEN-1 : i>=w+x2 : i--)
            a_buffer->i = a_buffer->(i-x2+w2);

        ! ...increasing buffer size accordingly.
        SetKeyBufLength(GetKeyBufLength(a_buffer) + (x2-w2), a_buffer);
    }

    ! Write the correction in:

    for (i=0 : i<x2 : i++) a_buffer->(i+w) = buffer2->(i+x1);

    Tokenise__(a_buffer, a_table);
    nw=NumberWords(a_table);

    return nw;
]; ! end of Keyboard

#Ifdef TARGET_ZCODE;

! Platform-independent way of printing strings, routines and properties
! to a buffer (defined as length word followed by byte characters).

[ PrintToBuffer buf len a b c d e;
    print_anything_result = 0;
    @output_stream 3 buf;
    switch (metaclass(a)) {
      String:
        print (string) a;
      Routine:
        print_anything_result = a(b, c, d, e);
      Object,Class:
        if (b)
            print_anything_result = PrintOrRun(a, b, true);
        else
            print (name) a;
    }
    @output_stream -3;
    if (buf-->0 > len) RunTimeError(14, len, "in PrintToBuffer()");
    return buf-->0;
];

#Ifnot; ! TARGET_GLULX

[ PrintToBuffer buf len a b c d e;
    if (b) {
        if (metaclass(a) == Object && a.#b == WORDSIZE
            && metaclass(a.b) == String)
            buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a.b);
        else
            buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a, b, c, d, e);
    }
    else
        buf-->0 = PrintAnyToArray(buf+WORDSIZE, len, a);
    if (buf-->0 > len) buf-->0 = len;
    return buf-->0;
];

#Endif; ! TARGET_

! Print contents of buffer (defined as length word followed by byte characters).
! no_break == 1: omit trailing newline.
! set_case == 1: capitalise first letter;
!          == 2: capitalise first letter, remainder lower case;
!          == 3: all lower case;
!          == 4: all upper case.
! centred == 1:  add leading spaces.

[ PrintFromBuffer buf no_break set_case centred
    i j k;
    j = (buf-->0) - 1;
    if (buf->(j+WORDSIZE) ~= 10 or 13) j++;     ! trim any trailing newline
    if (centred) {
        k = (ScreenWidth() - j) / 2;
        if (k>0) spaces k;
    }
    for (i=0 : i<j : i++) {
        k = buf->(WORDSIZE+i);
        switch (set_case) {
          0:    break;
          1:    if (i) set_case = 0;
                else   k = UpperCase(k);
          2:    if (i) k = LowerCase(k);
                else   k = UpperCase(k);
          3:           k = LowerCase(k);
          4:           k = UpperCase(k);
        }
        print (char) k;
    }
    if (no_break == false) new_line;
    return j;
];

[ Indefart o saveIndef;
    saveIndef = indef_mode; indef_mode = true; caps_mode = false;
    if (o has proper) {
        indef_mode = NULL;
        print (PSN__) o;
        indef_mode = saveIndef;
        return;
    }

    if (o provides article) {
        PrintOrRun(o, article, 1);
        print " ", (PSN__) o;
        indef_mode = saveIndef;
        return;
    }
    PrefaceByArticle(o, 2);
    indef_mode = saveIndef;
];

[ CInDefArt o saveIndef saveCaps;
    saveIndef = indef_mode; indef_mode = true;
    saveCaps = caps_mode; caps_mode = true;

    if (o has proper) {
        indef_mode = NULL;
        PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, PSN__, o);
        PrintFromBuffer(StorageForShortName, true, caps_mode);
        caps_mode = saveCaps;
        return;
    }

    if (o provides article) {
        PrintCapitalised(o, article, true);
        print " ", (PSN__) o;
        indef_mode = saveIndef;
        caps_mode = saveCaps;
        return;
    }
    PrefaceByArticle(o, 2, 0, 1);
    caps_mode = saveCaps;
    indef_mode = saveIndef;
];

[ Defart o saveIndef;
    saveIndef = indef_mode;
    indef_mode = false;
    caps_mode = false;
    if ((~~o ofclass Object) || o has proper) {
        indef_mode = NULL;
        print (PSN__) o;
        indef_mode = saveIndef;
        return;
    }
    PrefaceByArticle(o, 1);
    indef_mode = saveIndef;
];

[ CDefart o saveIndef saveCaps;
    #Ifdef YOU__TX;
    if (o == player) {
        print (string) YOU__TX;
        return;
    }
    #Endif;
    saveIndef = indef_mode; indef_mode = false;
    saveCaps = caps_mode; caps_mode = true;
    if (~~o ofclass Object) {
        indef_mode = NULL; print (PSN__) o;
    }
    else
        if (o has proper) {
            indef_mode = NULL;
            PrintToBuffer(StorageForShortName, SHORTNAMEBUF_LEN, PSN__, o);
            PrintFromBuffer(StorageForShortName, true, caps_mode);
        }
        else
            PrefaceByArticle(o, 0);
    indef_mode = saveIndef; caps_mode = saveCaps;
];

! ==============================================================================
