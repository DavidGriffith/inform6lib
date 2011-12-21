! ----------------------------------------------------------------------------
!  Imported library globals
! ----------------------------------------------------------------------------

Import global location;
Import global sline1;
Import global sline2;

Import global the_time;
Import global time_rate;
Import global time_step;

Import global score;
Import global turns;
Import global player;

Import global lightflag;
Import global real_location;
Import global print_player_flag;
Import global deadflag;

Import global transcript_mode;

Import global last_score;
Import global notify_mode;       ! Score notification

Import global places_score;
Import global things_score;
Import global lookmode;
Import global lastdesc;

Import global top_object;
Import global toomany_flag   ;          ! Flag for "take all made too many"
Import global actor          ;          ! Person asked to do something
Import global actors_location;          ! Like location, but for the actor
Import global action         ;          ! Thing he is asked to do
Import global inp1           ;          ! First parameter
Import global inp2           ;          ! Second parameter
Import global self           ;          ! Object whose routines are being run
Import global noun           ;          ! First noun
Import global second         ;          ! Second noun
Import global special_word   ;          ! Dictionary address of "special"
Import global special_number ;          ! The number, if a number was typed
Import global special_number2;          ! Second number, if two numbers typed
Import global parsed_number  ;          ! For user-supplied parsing routines
Import global multiflag;                    ! Multiple-object flag
Import global notheld_mode ;            ! To do with implicit taking
Import global onotheld_mode;            !
Import global meta;                         ! Verb is a meta-command (such as "save")
Import global reason_code;                  ! Reason for calling a life
Import global sw__var        ;          ! Switch variable (used for embeddeds)
Import global consult_from;                 ! Word that "consult" topic starts on
Import global consult_words;                ! ...and number of words in topic

#IFV5;
Import global undo_flag;                ! Can the interpreter provide "undo"?
#ENDIF;

Import global parser_trace;             ! Set this to 1 to make the parser trace
                                     ! tokens and lines
Import global debug_flag;               ! For debugging information
Import global lm_n;                         ! Parameters for LibraryMessages
Import global lm_o;

Import global wn;                      ! Word number (counts from 1)
Import global num_words;               ! Number of words typed
Import global verb_word;               ! Verb word (eg, take in "take all" or
                                ! "dwarf, take all") - address in dictionary
Import global verb_wordnum;            ! and the number in typing order (eg, 1 or 3)

Import global multi_mode;              ! Multiple mode
Import global multi_wanted;            ! Number of things needed in multitude
Import global multi_had;               ! Number of things actually found
Import global multi_context;           ! What token the multi-object was accepted for

Import global pcount;                  ! and a marker within it
Import global pcount2;                 ! so far

Import global parameters;              ! Parameters (objects) entered so far
Import global params_wanted;           ! Number needed (may change in parsing)

Import global nsns;                    ! Number of special_numbers entered so far

Import global inferfrom;               ! The point from which the rest of the
                                ! command must be inferred
Import global inferword;               ! And the preposition inferred

Import global oops_from;           ! The "first mistake" point, where oops acts
Import global saved_oops;          ! Used in working this out
Import global number_matched;          ! How many items in it?  (0 means none)
Import global number_of_classes;       ! How many equivalence classes?
Import global match_length;            ! How many typed words long are these matches?
Import global match_from;              ! At what word of the input do they begin?

Import global parser_action;           ! For the use of the parser when calling
Import global parser_one;              ! user-supplied routines
Import global parser_two;              !

Import global vague_word;              ! Records which vague word ("it", "them", ...)
                                ! caused an error
Import global vague_obj;               ! And what it was thought to refer to

Import global itobj;                   ! The object which is currently "it"
Import global himobj;                  ! The object which is currently "him"
Import global herobj;                  ! The object which is currently "her"

Import global lookahead;               ! The token after the object now being matched
Import global indef_mode;              ! "Indefinite" mode - ie, "take a brick" is in
                                ! this mode
Import global indef_type;              ! Bit-map holding types of specification
Import global indef_wanted;            ! Number of items wanted (100 for all)
Import global indef_guess_p;           ! Plural-guessing flag
Import global allow_plurals;           ! Whether they are presently allowed or not
Import global not_holding;             ! Object to be automatically taken as an
                                ! implicit command
Import global saved_wn;                ! These are temporary variables for Parser()
Import global saved_token;             ! (which hasn't enough spare local variables)

Import global held_back_mode;      ! Flag: is there some input from last time
Import global hb_wn;               ! left over?  (And a save value for wn)

Import global best_etype;              ! Error number used within parser
Import global etype;                   ! Error number used for individual lines

Import global last_command_from;       ! For sorting out "then again"
Import global last_command_to;         !

Import global token_was;               ! For noun filtering by user routines

Import global advance_warning;         ! What a later-named thing will be

Import global placed_in_flag;          ! To do with PlaceInScope
Import global length_of_noun;          ! Set by NounDomain to number of words in noun

Import global action_to_be;            ! So the parser can "cheat" in one case
Import global dont_infer;              ! Another dull flag

Import global scope_reason;   ! For "each_turn" and reactions

Import global scope_token;             ! For scope:Routine tokens
Import global scope_error;
Import global scope_stage;

Import global ats_flag;            ! For AddToScope routines
Import global ats_hls;                 !

Import global take_all_rule;
Import global usual_grammar_after;

#IFV5;
Import global just_undone;         ! Can't have two successive UNDOs
#ENDIF;

#IFV5;
Import global pretty_flag;
Import global menu_nesting;
#ENDIF;
Import global item_width;
Import global item_name;
Import global menu_item;

Import global active_timers;

Import global inventory_stage;
Import global c_style;
Import global lt_value;
Import global listing_together;
Import global listing_size;
Import global inventory_style;
Import global keep_silent;
Import global receive_action;
Import global wlf_indent;
#ifdef DEBUG;
Import global xcommsdir;
Import global x_scope_count;
#endif;
