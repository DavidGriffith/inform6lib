! ----------------------------------------------------------------------------
!  LINKLPA:  Link declarations of common properties and attributes.
!
!  Supplied for use with Inform 6                         Serial number 960912
!                                                                  Release 6/2
!  (c) Graham Nelson 1993, 1994, 1995, 1996 but freely usable (see manuals)
! ----------------------------------------------------------------------------

System_file;

Attribute animate;
Ifdef USE_MODULES;
   Iffalse (animate==0);
   Message error "Please move your Attribute declarations after the
                  Include ~Parser~ line: otherwise it will be impossible
                  to USE_MODULES";
   Endif;
Endif;
Attribute clothing;
Attribute concealed;
Attribute container;
Attribute door;
Attribute edible;
Attribute enterable;
Attribute female;
Attribute general;
Attribute light;
Attribute lockable;
Attribute locked;
Attribute moved;
Attribute on;
Attribute open;
Attribute openable;
Attribute proper;
Attribute scenery;
Attribute scored;
Attribute static;
Attribute supporter;
Attribute switchable;
Attribute talkable;
Attribute transparent;
Attribute visited;
Attribute workflag;
Attribute worn;

Attribute absent alias female;      !  Please, no psychoanalysis

Property additive before $ffff;
Ifdef USE_MODULES;
   Iffalse before==4;
   Message error "Please move your Property declarations after the
                  Include ~Parser~ line: otherwise it will be impossible
                  to USE_MODULES";
   Endif;
Endif;
Property additive after  $ffff;
Property additive life   $ffff;

Property long n_to;  Property long s_to; !  Slightly wastefully, these are
Property long e_to;  Property long w_to; !  long (they might be routines)
Property long ne_to; Property long se_to;
Property long nw_to; Property long sw_to;
Property long u_to;  Property long d_to;
Property long in_to; Property long out_to;

Property door_to     alias n_to;     !  For economy: these properties are
Property when_closed alias s_to;     !  used only by objects which
Property with_key    alias e_to;     !  aren't rooms
Property door_dir    alias w_to;
Property invent      alias u_to;
Property plural      alias d_to;
Property add_to_scope alias se_to;
Property list_together alias sw_to;
Property react_before alias ne_to;
Property react_after  alias nw_to;
Property grammar     alias in_to;
Property orders      alias out_to;

Property long initial;
Property when_open   alias initial;
Property when_on     alias initial;
Property when_off    alias when_closed;
Property long description;
Property additive describe $ffff;
Property article "a";

Property cant_go "You can't go that way.";

Property long found_in;         !  For fiddly reasons this can't alias

Property long time_left;
Property long number;
Property additive time_out $ffff;
Property daemon alias time_out;
Property additive each_turn $ffff;

Property capacity 100;

Property long short_name 0;
Property long parse_name 0;

