! ----------------------------------------------------------------------------
!  "VERBLIB" module file for Inform 6
!
!  (c) Graham Nelson 1993, 1994, 1995, but freely usable (see documentation)
! ----------------------------------------------------------------------------

#IFDEF MODULE_MODE;
Constant DEBUG;
Include "linklpa";
Include "linklv";
#ENDIF;

System_file;

! ----------------------------------------------------------------------------
!  First, printing a number n in English.
!  It's this sort of thing which makes you realise just what an irregular
!  language English is...
! ----------------------------------------------------------------------------

[ EnglishNumber n m f;
  if (n==0) { print "zero"; rfalse; }
  if (n<0) { print "minus "; n=-n; }
  if (n>=1000) { EnglishNumber(n/1000); print " thousand"; n=n%1000; f=1; }
  if (n>=100)  { if (f==1) print ", ";
                 EnglishNumber(n/100); print " hundred"; n=n%100; f=1; }
  if (n==0) rfalse;
  if (f==1) print " and ";            ! Americans may want to delete this line
  if (n<10) { EnglishDigit(n); rfalse; }
  if (n>=20)
  {   m=n/10;
      if (m==2) print "twenty";
      if (m==3) print "thirty";
      if (m==4) print "forty";
      if (m==5) print "fifty";
      if (m==6) print "sixty";
      if (m==7) print "seventy";
      if (m==8) print "eighty";
      if (m==9) print "ninety";
      if (n%10==0) rfalse;
      print "-"; EnglishDigit(n%10); rfalse;
  }
  if (n==10) { print "ten"; rfalse; }
  if (n==11) { print "eleven"; rfalse; }
  if (n==12) { print "twelve"; rfalse; }
  if (n==13) { print "thirteen"; rfalse; }
  if (n==14) { print "fourteen"; rfalse; }
  if (n==15) { print "fifteen"; rfalse; }
  if (n==16) { print "sixteen"; rfalse; }
  if (n==17) { print "seventeen"; rfalse; }
  if (n==18) { print "eighteen"; rfalse; }
  print "nineteen";
];

[ EnglishDigit n;
  if (n==1) { print "one"; rfalse; }
  if (n==2) { print "two"; rfalse; }
  if (n==3) { print "three"; rfalse; }
  if (n==4) { print "four"; rfalse; }
  if (n==5) { print "five"; rfalse; }
  if (n==6) { print "six"; rfalse; }
  if (n==7) { print "seven"; rfalse; }
  if (n==8) { print "eight"; rfalse; }
  if (n==9) { print "nine"; rfalse; }
];

! ----------------------------------------------------------------------------
!  Next the WriteListFrom routine, a flexible object-lister taking care of
!  plurals, inventory information, various formats and so on.  This is used
!  by everything in the library which ever wants to list anything.
!
!  If there were no objects to list, it prints nothing and returns false;
!  otherwise it returns true.
!
!  o is the object, and style is a bitmap, whose bits are given by:
! ----------------------------------------------------------------------------

Constant NEWLINE_BIT    1;    !  New-line after each entry
Constant INDENT_BIT     2;    !  Indent each entry by depth
Constant FULLINV_BIT    4;    !  Full inventory information after entry
Constant ENGLISH_BIT    8;    !  English sentence style, with commas and and
Constant RECURSE_BIT   16;    !  Recurse downwards with usual rules
Constant ALWAYS_BIT    32;    !  Always recurse downwards
Constant TERSE_BIT     64;    !  More terse English style
Constant PARTINV_BIT  128;    !  Only brief inventory information after entry
Constant DEFART_BIT   256;    !  Use the definite article in list
Constant WORKFLAG_BIT 512;    !  At top level (only), only list objects
                              !  which have the "workflag" attribute
Constant ISARE_BIT   1024;    !  Print " is" or " are" before list
Constant CONCEAL_BIT 2048;    !  Omit objects with "concealed" or "scenery":
                              !  if WORKFLAG_BIT also set, then does _not_
                              !  apply at top level, but does lower down
Constant NOARTICLE_BIT 4096;  !  Print no articles, definite or not

[ NextEntry o odepth;
  o=sibling(o);
  if (lt_value~=0)
  {   if (c_style & WORKFLAG_BIT ~= 0 && odepth==0)
      {   while (o~=0 && (o.list_together~=lt_value
                          || o hasnt workflag)) o=sibling(o);
          return o;
      }
      if (c_style & CONCEAL_BIT ~= 0)
      {   while (o~=0 &&
                 (o has concealed || o has scenery
                  || o.list_together~=lt_value)) o=sibling(o);
          return o;
      }
      while (o~=0 && o.list_together~=lt_value) o=sibling(o);
      return o;
  }
  if (c_style & WORKFLAG_BIT ~= 0 && odepth==0)
  {   while (o~=0 && o hasnt workflag) o=sibling(o);
      return o;
  }
  if (c_style & CONCEAL_BIT ~= 0)
      while (o~=0 && (o has concealed || o has scenery)) o=sibling(o);
  return o;
];

[ WillRecurs o;
  if (c_style & ALWAYS_BIT ~= 0) rtrue;
  if (c_style & RECURSE_BIT == 0) rfalse;
  if (o has transparent
      || o has supporter
      || (o has container && o has open)) rtrue;
  rfalse;
];

[ ListEqual o1 o2;
  if (child(o1)~=0 && WillRecurs(o1)~=0) rfalse;
  if (child(o2)~=0 && WillRecurs(o2)~=0) rfalse;

  if (c_style & (FULLINV_BIT + PARTINV_BIT) ~= 0)
  {   if ((o1 hasnt worn && o2 has worn)
          || (o2 hasnt worn && o1 has worn)) rfalse;
      if ((o1 hasnt light && o2 has light)
          || (o2 hasnt light && o1 has light)) rfalse;
  }

  return Identical(o1,o2);
];

[ SortTogether obj value;
!  print "Sorting together possessions of ",
!         object obj, " by value ", value, "^";
  while (child(obj)~=0)
  {   if (child(obj).list_together~=value) move child(obj) to out_obj;
      else move child(obj) to in_obj;
  }
  while (child(in_obj)~=0)
      move child(in_obj) to obj;
  while (child(out_obj)~=0)
      move child(out_obj) to obj;
];

[ SortOutList obj i k l;
!  print "^^Sorting out list from ", (name) obj, "^  ";
!  for (i=child(location):i~=0:i=sibling(i))
!      print (name) i, " --> ";
!  new_line;
 .AP_SOL;
  for (i=obj:i~=0:i=sibling(i))
  {   k=i.list_together;
      if (k~=0)
      {   ! print "Scanning ", (name) i, " with lt=", k, "^";
          for (i=sibling(i):i~=0 && i.list_together==k:) i=sibling(i);
              if (i==0) rfalse;
          ! print "First not in block is ", (name) i,
          ! " with lt=", i.list_together, "^";
          for (l=sibling(i):l~=0:l=sibling(l))
              if (l.list_together==k)
              {   SortTogether(parent(obj), k);
!  print "^^After ST:^  ";
!  for (i=child(location):i~=0:i=sibling(i))
!      print (name) i, " --> ";
!  new_line;
                  obj = child(parent(obj));
                  jump AP_SOL;
              }
      }
  }
];

[ WriteListFrom o style depth;
  if (o==child(parent(o)))
  {   SortOutList(o); o=child(parent(o)); }
  c_style=style;
  if (style & WORKFLAG_BIT ~= 0)
  {   while (o~=0 && o hasnt workflag) o=sibling(o);
  }
  else
  {   if (c_style & CONCEAL_BIT ~= 0)
          while (o~=0 && (o has concealed || o has scenery)) o=sibling(o);
  }
  if (o==0) rfalse;
  wlf_indent=0; WriteListR(o,depth);
  rtrue;
];

[ WriteListR o depth stack_pointer  classes_p sizes_p i j k k2 l m n q senc mr;

  if (depth>0 && o==child(parent(o)))
  {   SortOutList(o); o=child(parent(o)); }

  classes_p = match_classes + stack_pointer;
  sizes_p   = match_list + stack_pointer;

  for (i=o,j=0:i~=0 && (j+stack_pointer)<128:i=NextEntry(i,depth),j++)
  {   classes_p->j=0;
      if (i.plural~=0) k++;
  }

  if (c_style & ISARE_BIT ~= 0)
  {   if (j==1) print " is"; else print " are";
      if (c_style & NEWLINE_BIT ~= 0) print ":^"; else print (char) ' ';
      c_style = c_style - ISARE_BIT;
  }

  stack_pointer = stack_pointer+j+1;

  if (k<2) jump EconomyVersion;   ! It takes two to plural

  n=1;
  for (i=o,k=0:k<j:i=NextEntry(i,depth),k++)
      if (classes_p->k==0)
      {   classes_p->k=n; sizes_p->n=1;
          for (l=NextEntry(i,depth), m=k+1:l~=0 && m<j:
               l=NextEntry(l,depth), m++)
              if (classes_p->m==0 && i.plural~=0 && l.plural~=0)
              {   if (ListEqual(i,l)==1)
                  {   sizes_p->n = sizes_p->n + 1;
                      classes_p->m = n;
                  }
              }
          n++;
      }
  n--;

  for (i=1, j=o, k=0: i<=n: i++, senc++)
  {   while (((classes_p->k) ~= i)
             && ((classes_p->k) ~= -i)) { k++; j=NextEntry(j,depth); }
      m=sizes_p->i;
      if (j.list_together~=0 or lt_value
          && ZRegion(j.list_together)==2 or 3
          && j.list_together==mr) senc--;
      mr=j.list_together;
  }
  senc--;
  for (i=1, j=o, k=0, mr=0: senc>=0: i++, senc--)
  {   while (((classes_p->k) ~= i)
             && ((classes_p->k) ~= -i)) { k++; j=NextEntry(j,depth); }
      if (j.list_together~=0 or lt_value)
      {   if (j.list_together==mr) { senc++; jump Omit_FL2; }
          k2=NextEntry(j,depth);
          if (k2==0 || k2.list_together~=j.list_together) jump Omit_WL2;
          k2=ZRegion(j.list_together);
          if (k2==2 or 3)
          {   q=j; listing_size=1; l=k; m=i;
              while (m<n && q.list_together==j.list_together)
              {   m++;
                  while (((classes_p->l) ~= m)
                         && ((classes_p->l) ~= -m))
                  {   l++; q=NextEntry(q,depth); }
                  if (q.list_together==j.list_together) listing_size++;
              }
!              print " [", listing_size, "] ";
              if (listing_size==1) jump Omit_WL2;
              if (c_style & INDENT_BIT ~= 0) spaces 2*(depth+wlf_indent);
              if (k2==3)
              {   q=0; for (l=0:l<listing_size:l++) q=q+sizes_p->(l+i);
                  EnglishNumber(q); print " ";
                  print (string) j.list_together;
                  if (c_style & ENGLISH_BIT ~= 0) print " (";
                  if (c_style & INDENT_BIT ~= 0) print ":^";
              }
              q=c_style;
              if (k2~=3)
              {   inventory_stage=1;
                  parser_one=j; parser_two=depth+wlf_indent;
                  if (RunRoutines(j,list_together)==1) jump Omit__Sublist2;
              }
              lt_value=j.list_together; listing_together=j; wlf_indent++;
              WriteListR(j,depth,stack_pointer); wlf_indent--;
              lt_value=0; listing_together=0;
              if (k2==3)
              {   if (q & ENGLISH_BIT ~= 0) print ")";
              }
              else
              {   inventory_stage=2;
                  parser_one=j; parser_two=depth+wlf_indent;
                  RunRoutines(j,list_together);
              }
             .Omit__Sublist2;
              if (q & NEWLINE_BIT ~= 0 && c_style & NEWLINE_BIT == 0) new_line;
              c_style=q;
              mr=j.list_together;
              jump Omit_EL2;
          }
      }

     .Omit_WL2;
      if (WriteBeforeEntry(j,depth)==1) jump Omit_FL2;
      if (sizes_p->i == 1)
      {   if (c_style & NOARTICLE_BIT ~= 0) PrintShortName(j);
          else
          {   if (c_style & DEFART_BIT ~= 0) DefArt(j); else InDefArt(j);
          }
      }
      else
      {   if (c_style & DEFART_BIT ~= 0) print "the ";
          EnglishNumber(sizes_p->i); print " ";
          PrintOrRun(j,plural,1);
      }
      WriteAfterEntry(j,depth,stack_pointer);

     .Omit_EL2;
      if (c_style & ENGLISH_BIT ~= 0)
      {   if (senc==1) print " and ";
          if (senc>1) print ", ";
      }
     .Omit_FL2;
  }
  rtrue;

  .EconomyVersion;

  n=j;

  for (i=1, j=o: i<=n: j=NextEntry(j,depth), i++, senc++)
  {   if (j.list_together~=0 or lt_value
          && ZRegion(j.list_together)==2 or 3
          && j.list_together==mr) senc--;
      mr=j.list_together;
  }

  for (i=1, j=o, mr=0: i<=senc: j=NextEntry(j,depth), i++)
  {   if (j.list_together~=0 or lt_value)
      {   if (j.list_together==mr) { i--; jump Omit_FL; }
          k=NextEntry(j,depth);
          if (k==0 || k.list_together~=j.list_together) jump Omit_WL;
          k=ZRegion(j.list_together);
          if (k==2 or 3)
          {   if (c_style & INDENT_BIT ~= 0) spaces 2*(depth+wlf_indent);
              if (k==3)
              {   q=j; l=0;
                  do
                  {   q=NextEntry(q,depth); l++;
                  } until (q.list_together~=j.list_together);
                  EnglishNumber(l); print " ";
                  print (string) j.list_together;
                  if (c_style & ENGLISH_BIT ~= 0) print " (";
                  if (c_style & INDENT_BIT ~= 0) print ":^";
              }
              q=c_style;
              if (k~=3)
              {   inventory_stage=1;
                  parser_one=j; parser_two=depth+wlf_indent;
                  if (RunRoutines(j,list_together)==1) jump Omit__Sublist;
              }
              lt_value=j.list_together; listing_together=j; wlf_indent++;
              WriteListR(j,depth,stack_pointer); wlf_indent--;
              lt_value=0; listing_together=0;
              if (k==3)
              {   if (q & ENGLISH_BIT ~= 0) print ")";
              }
              else
              {   inventory_stage=2;
                  parser_one=j; parser_two=depth+wlf_indent;
                  RunRoutines(j,list_together);
              }
             .Omit__Sublist;
              if (q & NEWLINE_BIT ~= 0 && c_style & NEWLINE_BIT == 0) new_line;
              c_style=q;
              mr=j.list_together;
              jump Omit_EL;
          }
      }
     .Omit_WL;
      if (WriteBeforeEntry(j,depth)==1) jump Omit_FL;
      if (c_style & NOARTICLE_BIT ~= 0) PrintShortName(j);
      else
      {   if (c_style & DEFART_BIT ~= 0) DefArt(j); else InDefArt(j);
      }
      WriteAfterEntry(j,depth,stack_pointer);

     .Omit_EL;
      if (c_style & ENGLISH_BIT ~= 0)
      {   if (i==senc-1) print " and ";
          if (i<senc-1) print ", ";
      }
     .Omit_FL;
  }
];

[ WriteBeforeEntry o depth  flag;
  if (c_style & INDENT_BIT ~= 0) spaces 2*(depth+wlf_indent);

  if (c_style & FULLINV_BIT ~= 0)
  {   if (o.invent~=0)
      {   inventory_stage=1;
          flag=PrintOrRun(o,invent,1);
          if (flag==1 && c_style & NEWLINE_BIT ~= 0) new_line;
      }
  }
  return flag;
];

[ WriteAfterEntry o depth stack_p  flag flag2 flag3 p comb;

  if (c_style & PARTINV_BIT ~= 0)
  {   comb=0;
      if (o has light && location hasnt light) comb=comb+1;
      if (o has container && o hasnt open)     comb=comb+2;
      if ((o has container && (o has open || o has transparent))
          && (child(o)==0)) comb=comb+4;
      if (comb==1) print " (providing light)";
      if (comb==2) print " (which is closed)";
      if (comb==3) print " (closed and providing light)";
      if (comb==4) print " (which is empty)";
      if (comb==5) print " (empty and providing light)";
      if (comb==6) print " (which is closed and empty)";
      if (comb==7) print " (closed, empty and providing light)";
  }

  if (c_style & FULLINV_BIT ~= 0)
  {   if (o.invent ~= 0)
      {   inventory_stage=2;
          if (RunRoutines(o,invent)~=0)
          {   if (c_style & NEWLINE_BIT ~= 0) new_line;
              rtrue;
          }
      }
      if (o has light && o has worn)
      {   print " (providing light and being worn"; flag2=1; }
      else
      {   if (o has light) { print " (providing light"; flag2=1; }
          if (o has worn)  { print " (being worn"; flag2=1; }
      }
      if (o has container)
      {   if (o has openable)
          {   if (flag2==1) print " and ";
              else print " (which is ";
              if (o has open)
              {   print "open";
                  if (child(o)==0) print " but empty";
              }
              else print "closed";
              if (o has lockable && o has locked) print " and locked";
              flag2=1;
          }
          else
              if (child(o)==0)
              {   if (flag2==1) print " and empty";
                  else print " (which is empty)";
              }
      }
      if (flag2==1) print ")";
  }

  if (c_style & CONCEAL_BIT == 0)
  {   if (child(o)~=0) flag3=children(o);
  }
  else
  {   objectloop (p in o)
          if (p hasnt concealed) flag3++;
  }

  if (c_style & ALWAYS_BIT ~= 0 && flag3>0)
  {   if (c_style & ENGLISH_BIT ~= 0) print " containing ";
      flag=1;
  }

  if (c_style & RECURSE_BIT ~= 0 && flag3>0)
  {   if (o has supporter)
      {   if (c_style & ENGLISH_BIT ~= 0)
          {   if (c_style & TERSE_BIT ~= 0)
              print " (on "; else print ", on top of ";
              if (o has animate) print "whom "; else print "which ";
          }
          flag=1;
      }
      if (o has container && (o has open || o has transparent))
      {   if (c_style & ENGLISH_BIT ~= 0)
          {   if (c_style & TERSE_BIT ~= 0)
                  print " (in "; else print ", inside ";
              if (o has animate) print "whom "; else print "which ";
          }
          flag=1;
      }
  }

  if (flag==1 && c_style & ENGLISH_BIT ~= 0)
  {   if (flag3 > 1) print "are "; else print "is ";
  }

  if (c_style & NEWLINE_BIT ~= 0) new_line;

  if (flag==1) WriteListR(child(o),depth+1,stack_p);

  if (flag==1 && c_style & TERSE_BIT ~= 0) print ")";
];

! ----------------------------------------------------------------------------
!   A cunning routine (which could have been a daemon, but isn't, for the
!   sake of efficiency) to move objects which could be in many rooms about
!   so that the player never catches one not in place
! ----------------------------------------------------------------------------

[ MoveFloatingObjects i k l m address;
  for (i=selfobj+1: i<=top_object: i++)
  {   address=i.&found_in;
      if (address~=0 && i hasnt absent)
      {   if (ZRegion(address-->0)==2)
          {   if (indirect(address-->0) ~= 0) move i to location;
          }
          else
          {   k=i.#found_in;
              for (l=0: l<k/2: l++)
              {   m=address-->l;
                  if (m==location || m in location) move i to location;
              }
          }
      }
  }
];

! ----------------------------------------------------------------------------
!   Two little routines for moving the player safely.
! ----------------------------------------------------------------------------

[ PlayerTo newplace flag;
  move player to newplace;
  while (parent(newplace)~=0) newplace=parent(newplace);
  location=newplace;
  real_location=location;
  AdjustLight(1);
  if (flag==0) <Look>;
  if (flag==1) { NoteArrival(); ScoreArrival(); }
  if (flag==2) LookSub(1);
];

[ MovePlayer direc; <Go direc>; <Look>; ];

! ----------------------------------------------------------------------------
!   The handy YesOrNo routine, and some "meta" verbs
! ----------------------------------------------------------------------------

[ YesOrNo i;
  for (::)
  {
   #IFV3; read buffer parse; #ENDIF;
   #IFV5; read buffer parse DrawStatusLine; #ENDIF;
      i=parse-->1;
      if (i=='yes' or #n$y) rtrue;
      if (i=='no' or #n$n) rfalse;
      L__M(##Quit,1); print "> ";
  }
];

[ QuitSub; L__M(##Quit,2); if (YesOrNo()~=0) quit; ];

[ RestartSub; L__M(##Restart,1);
  if (YesOrNo()~=0) { @restart; L__M(##Restart,2); }
];

[ RestoreSub;
  restore Rmaybe;
  return L__M(##Restore,1);
  .RMaybe; L__M(##Restore,2);
];

[ SaveSub;
  save Smaybe;
  return L__M(##Save,1);
  .SMaybe; L__M(##Save,2);
];

[ VerifySub;
  @verify ?Vmaybe;
  jump Vwrong;
  .Vmaybe; return L__M(##Verify,1);
  .Vwrong;
  L__M(##Verify,2);
];

[ ScriptOnSub;
  if (transcript_mode==1) return L__M(##ScriptOn,1);
  transcript_mode=1;
  0-->8 = (0-->8)|1;
  L__M(##ScriptOn,2); VersionSub();
];

[ ScriptOffSub;
  if (transcript_mode==0) return L__M(##ScriptOff,1);
  L__M(##ScriptOff,2);
  transcript_mode=0;
  0-->8 = (0-->8)&$fffe;
];

[ NotifyOnSub; notify_mode=1; L__M(##NotifyOn); ];
[ NotifyOffSub; notify_mode=0; L__M(##NotifyOff); ];

#IFNDEF NO_PLACES;
[ PlacesSub i j k;
  L__M(##Places);
  for (i=selfobj:i<=top_object:i++)
      if (i has visited) j++;

  for (i=selfobj:i<=top_object:i++)
  {   if (i has visited)
      {   PrintShortName(i); k++;
          if (k==j) ".";
          if (k==j-1) print " and "; else print ", ";
      }
  }
];
[ ObjectsSub i j f;
  L__M(##Objects,1);
  for (i=selfobj:i<=top_object:i++)
  {   if (i has moved)
      {   f=1; DefArt(i); j=parent(i);
          if (j==player)
          {   if (i has worn) print "   (worn)";
              else print "   (held)";
              jump obj__ptd;
          }
          if (j has animate) { print "   (given away)"; jump obj__ptd; }
          if (j has visited) { print "   (in "; PrintShortName(j);
                               print ")"; jump obj__ptd; }
          if (j has enterable) { print "   (in "; DefArt(j); print ")";
                                 jump obj__ptd; }
          if (j has container) { print "   (inside "; PrintShortName(j);
                                 print ")"; jump obj__ptd; }
          if (j has supporter) { print "   (on "; PrintShortName(j);
                                 print ")"; jump obj__ptd; }
          print "   (lost)";
          .obj__ptd; new_line;
      }
  }
  if (f==0) L__M(##Objects,2);
];
#ENDIF;

! ----------------------------------------------------------------------------
!   The scoring system
! ----------------------------------------------------------------------------

[ ScoreSub;
  L__M(##Score);
  PrintRank();
];

[ Achieved num;
  if (task_done->num==0)
  {   task_done->num=1; score=score+task_scores->num;
  }
];

[ PANum m n;
  print "  ";
  n=m;
  if (n<0)    { n=-m; n=n*10; }
  if (n<10)   { print "   "; jump panuml; }
  if (n<100)  { print "  "; jump panuml; }
  if (n<1000) { print " "; }
.panuml;
  print m, " ";
];

[ FullScoreSub i;
  ScoreSub();
  if (score==0 || TASKS_PROVIDED==1) rfalse;
  new_line;
  L__M(##FullScore,1);

  for (i=0:i<NUMBER_TASKS:i++)
      if (task_done->i==1)
      {   PANum(task_scores->i);
          PrintTaskName(i);
      }
  
  if (things_score~=0)
  {   PANum(things_score); L__M(##FullScore,2); }
  if (places_score~=0)
  {   PANum(places_score); L__M(##FullScore,3); }
  new_line; PANum(score); L__M(##FullScore,4);
];

! ----------------------------------------------------------------------------
!   Real verbs start here: Inventory
! ----------------------------------------------------------------------------

[ InvWideSub;
  inventory_style = FULLINV_BIT + ENGLISH_BIT + RECURSE_BIT;
  <Inv>;
];

[ InvTallSub;
  inventory_style = FULLINV_BIT + INDENT_BIT + NEWLINE_BIT + RECURSE_BIT;
  <Inv>;
];

[ InvSub;
  if (child(player)==0) return L__M(##Inv,1);
  if (inventory_style==0) <<InvTall>>;

  L__M(##Inv,2);
  if (inventory_style & NEWLINE_BIT ~= 0) print ":^"; else print " ";

  WriteListFrom(child(player), inventory_style, 1);
  if (inventory_style & ENGLISH_BIT ~= 0) print ".^";

  AfterRoutines();
];

! ----------------------------------------------------------------------------
!   Object movement verbs
! ----------------------------------------------------------------------------

[ TakeSub;
  if (onotheld_mode==0 || parent(noun)~=player)
  {   if (location==thedark)
      {   if (RTakeSub(real_location)~=0) rtrue;
      }
      else
      {   if (RTakeSub(location)~=0) rtrue;
      }
  }
  if (AfterRoutines()==1) rtrue;
  notheld_mode=onotheld_mode;
  if (notheld_mode==1 || keep_silent==1) rtrue;
  L__M(##Take,1);
];

[ RTakeSub fromobj i j k postonobj;
  if (noun==player) return L__M(##Take,2);

  if (noun has animate) return L__M(##Take,3,noun);

  if (parent(player)==noun) return L__M(##Take,4,noun);

  i=parent(noun);
  if (i==player) return L__M(##Take,5);

  if (i has container || i has supporter)
  {   postonobj=i;
      k=action; action=##LetGo;
      if (RunRoutines(i,before)~=0) { action=k; rtrue; }
      action=k;
  }

  while (i~=fromobj && i~=0)
  {   if (i hasnt container && i hasnt supporter)
      {   if (i has animate) return L__M(##Take,6,i);
          if (i has transparent) return L__M(##Take,7,i);
          return L__M(##Take,8);
      }
      if (i has container && i hasnt open)
          return L__M(##Take,9,i);
      i=parent(i);
      if (i==player) i=fromobj;
  }
  if (noun has scenery) return L__M(##Take,10);
  if (noun has static)  return L__M(##Take,11);

  k=0; objectloop (j in player) if (j hasnt worn) k++;

  if (k >= ValueOrRun(player,capacity))
  {   if (SACK_OBJECT~=0)
      {   if (parent(SACK_OBJECT)~=player)
              return L__M(##Take,12);
          j=0;
          objectloop (k in player) 
              if (k~=SACK_OBJECT && k hasnt worn && k hasnt light) j=k;

          if (j~=0)
          {   L__M(##Take,13,j);
              keep_silent = 1; <Insert j SACK_OBJECT>; keep_silent = 0;
              if (j notin SACK_OBJECT) rtrue;
          }
          else return L__M(##Take,12);
      }
      else return L__M(##Take,12);
  }
  move noun to player;

  if (postonobj~=0)
  {   k=action; action=##LetGo;
      if (RunRoutines(postonobj,after)~=0) { action=k; rtrue; }
      action=k;
  }
  rfalse;
];

[ DropSub i;
  i=parent(noun);
  if (i==location) return L__M(##Drop,1);
  if (i~=player) return L__M(##Drop,2);
  if (noun has worn)
  {   L__M(##Drop,3,noun);
      <Disrobe noun>;
      if (noun has worn) rtrue;
  }
  move noun to parent(player);
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  return L__M(##Drop,4);
];

[ RemoveSub i;
  i=parent(noun);
  if (i has container && i hasnt open) return L__M(##Remove,1);
  if (i~=second) return L__M(##Remove,2);
  if (i has animate) return L__M(##Take,6,i);
  if (RTakeSub(second)~=0) rtrue;
  action=##Take;   if (AfterRoutines()==1) rtrue;
  action=##Remove; if (AfterRoutines()==1) rtrue;

  if (keep_silent==1) rtrue;
  return L__M(##Remove,4);
];

[ IndirectlyContains o1 o2;  ! Does o1 already (ultimately) have o2?
  while (o2~=0)
  {   if (o1==o2) rtrue;
      o2=parent(o2);
  }
  rfalse;
];

[ PutOnSub;
  receive_action=##PutOn; 
  if (second==d_obj) { <Drop noun>; rfalse; }
  if (parent(noun)~=player) return L__M(##PutOn,1,noun);

  if (second>1)
  {   action=##Receive;
      if (RunRoutines(second,before)~=0) { action=##PutOn; rtrue; }
      action=##PutOn;
  }

  if (IndirectlyContains(noun,second)==1) return L__M(##PutOn,2);
  if (second hasnt supporter) return L__M(##PutOn,3,second);
  if (parent(second)==player) return L__M(##PutOn,4);
  if (noun has worn)
  {   L__M(##PutOn,5);
      <Disrobe noun>;
      if (noun has worn) rtrue;
  }
  if (children(second)>=ValueOrRun(second,capacity))
      return L__M(##PutOn,6,second);
  move noun to second;

  if (AfterRoutines()==1) rtrue;

  if (second>1)
  {   action=##Receive;
      if (RunRoutines(second,after)~=0) { action=##PutOn; rtrue; }
      action=##PutOn;
  }

  if (keep_silent==1) rtrue;
  if (multiflag==1) return L__M(##PutOn,7);
  L__M(##PutOn,8,noun);
];

[ InsertSub;
  receive_action = ##Insert;
  if (second==d_obj ) <<Drop noun>>;
  if (parent(noun)~=player) return L__M(##Insert,1);

  if (second>1)
  {   action=##Receive;
      if (RunRoutines(second,before)~=0) { action=##Insert; rtrue; }
      action=##Insert;
  }
  if (second hasnt container) return L__M(##Insert,2);
  if (second hasnt open)      return L__M(##Insert,3);
  if (IndirectlyContains(noun,second)==1) return L__M(##Insert,5);
  if (noun has worn)
  {   L__M(##Insert,6);
      <Disrobe noun>; if (noun has worn) rtrue;
  }

  if (children(second)>=ValueOrRun(second,capacity))
      return L__M(##Insert,7,second);

  move noun to second;

  if (AfterRoutines()==1) rtrue;

  if (second>1)
  {   action=##Receive;
      if (RunRoutines(second,after)~=0) { action=##Insert; rtrue; }
      action=##Insert;
  }
  if (keep_silent==1) rtrue;
  if (multiflag==1) return L__M(##Insert,8);
  L__M(##Insert,9,noun);
];

[ TransferSub i act_needed k postonobj par;
  act_needed=##Drop;
  if (second has container) act_needed=##Insert;
  else
      if (second has supporter) act_needed=##PutOn;

  i=parent(noun);
  if (i~=player)
  {   while (i~=0)
      {   if (i hasnt open) return L__M(##Transfer,1);
          i=parent(i);
          if (i==player) jump DoTransfer;
      }
      return L__M(##Transfer,2);
  }
  .DoTransfer;
  if (noun notin player)
  {   par = parent(noun);
      if (par has container || par has supporter)
      {   postonobj=par;
          k=action; action=##LetGo;
          if (RunRoutines(par,before)~=0) { action=k; rtrue; }
          action=k;
      }
      move noun to player;
      if (postonobj~=0)
      {   k=action; action=##LetGo;
          if (RunRoutines(postonobj,after)~=0) { action=k; rtrue; }
          action=k;
      }
  }
  if (act_needed==##Drop)   <<Drop noun>>;
  if (act_needed==##Insert) <<Insert noun second>>;
  if (act_needed==##PutOn)  <<PutOn noun second>>;

];

[ EmptySub;
  second=d_obj; EmptyTSub();
];

[ EmptyTSub i j;
  if (noun hasnt container) return L__M(##EmptyT,1,noun);
  if (noun hasnt open) return L__M(##EmptyT,2,noun);
  if (second~=d_obj)
  {   if (second hasnt container) return L__M(##EmptyT,1,second);
      if (second hasnt open) return L__M(##EmptyT,2,second);
  }
  i=child(noun);
  if (i==0) return L__M(##EmptyT,3,noun);
  while (i~=0)
  {   j=sibling(i);
      PrintShortName(i); print ": ";
      <Transfer i second>;
      i=j;
  }
];

[ GiveSub;
  if (parent(noun)~=player) return L__M(##Give,1,noun);
  if (second==player)  return L__M(##Give,2,noun);
  if (RunLife(second,##Give)~=0) rfalse;
  L__M(##Give,3,second);
];

[ GiveRSub; <Give second noun>; ];

[ ShowSub;
  if (parent(noun)~=player) return L__M(##Show,1,noun);
  if (second==player) <<Examine noun>>;
  if (RunLife(second,##Show)~=0) rfalse;
  L__M(##Show,2,second);
];

[ ShowRSub; <Show second noun>; ];

! ----------------------------------------------------------------------------
!   Travelling around verbs
! ----------------------------------------------------------------------------

[ EnterSub i;
  if (noun has door) <<Go noun>>;
  i=parent(player);
  if (i~=location) return L__M(##Enter,1,i);
  i=parent(noun);
  if (i==compass) <<Go noun>>;
  if (noun hasnt enterable) return L__M(##Enter,2);
  if (noun has container && noun hasnt open) return L__M(##Enter,3,noun);
  if (i~=location)  return L__M(##Enter,4);
  move player to noun;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Enter,5,noun);
  Locale(noun);
];

[ GetOffSub;
  if (parent(player)==noun) <<Exit>>;
  L__M(##GetOff,1,noun);
];

[ ExitSub p;
  p=parent(player);
  if (p==location || (location==thedark && p==real_location))
  {   if ((location.out_to~=0)
          || (location==thedark && real_location.out_to~=0)) <<Go out_obj>>;
      return L__M(##Exit,1);
  }
  if (p has container && p hasnt open)
      return L__M(##Exit,2,p);
  if (location == thedark) move player to real_location;
  else move player to location;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Exit,3,p); LookSub(1);
];

[ VagueGoSub; L__M(##VagueGo); ];

[ GoInSub;
  <<Go in_obj>>;
];

[ GoSub i j k df movewith thedir;

  movewith=0;
  i=parent(player);
  if ((location~=thedark && i~=location)
      || (location==thedark && i~=real_location))
  {   j=location;
      if (location==thedark) location=real_location;
      k=RunRoutines(i,before); if (k~=3) location=j;
      if (k==1)
      {   movewith=i; i=parent(i); jump gotroom; }
      if (k==0) L__M(##Go,1,i); rtrue;
  }
  .gotroom;
  thedir=noun.door_dir;
  if (ZRegion(thedir)==2) thedir=RunRoutines(noun,door_dir);
  
  j=i.thedir; k=ZRegion(j);
  if (k==3) { print (string) j; new_line; rfalse; }
  if (k==2) { j=RunRoutines(i,thedir);
              if (j==1) rtrue;
            }

  if (k==0 || j==0)
  {   if (i.cant_go ~= 0) PrintOrRun(i, cant_go);
      rfalse;
  }

  if (j has door)
  {   if (j has concealed) return L__M(##Go,2);
      if (j hasnt open)
      {   if (noun==u_obj) return L__M(##Go,3,j);
          if (noun==d_obj) return L__M(##Go,4,j);
          return L__M(##Go,5,j);
      }
      if (ZRegion(j.door_to)==2) j=RunRoutines(j,door_to);
      else j=j.door_to;
      if (j==0) return L__M(##Go,6,j);
      if (j==1) rtrue;
  }
  if (movewith==0) move player to j; else move movewith to j;

  df=OffersLight(j);
  if (df~=0) { location=j; lightflag=1; }
  else
  {   if (location==thedark) DarkToDark();
      real_location=j;
      location=thedark; lightflag=0;
  }
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  LookSub(1);
];

! ----------------------------------------------------------------------------
!   Describing the world.  SayWhatsOn(object) does just that (producing
!   no text if nothing except possibly "scenery" and "concealed" items are).
!   Locale(object) runs through the "tail end" of a Look-style room
!   description for the contents of the object, printing up suitable
!   descriptions as it goes.
! ----------------------------------------------------------------------------

[ SayWhatsOn descon j f;
  if (descon==parent(player)) rfalse;
  objectloop (j in descon)
      if (j hasnt concealed && j hasnt scenery) f=1;
  if (f==0) rfalse;
  L__M(##Look, 4, descon); rtrue;
];

[ Locale descin text1 text2  o p k j flag f2;

  objectloop (o in descin) give o ~workflag;

  k=0;
  objectloop (o in descin)
      if (o hasnt concealed && o~=parent(player))
      {  if (o hasnt scenery)
         {   give o workflag; k++;
             p=initial; f2=0;
             if (o has door && o hasnt open) { p=when_closed; f2=1; }
             if (o has switchable && o hasnt on) { p=when_off; f2=1; }
             if (o has container && o hasnt open && o.&when_closed~=0)
             {   p=when_closed; f2=1; }
             if (o hasnt moved || o.describe~=NULL || f2==1)
             {   if (o.describe~=NULL && RunRoutines(o,describe)~=0)
                 {   flag=1;
                     give o ~workflag; k--;
                 }    
                 else
                 {   j=o.p;
                     if (j~=0)
                     {   new_line;
                         PrintOrRun(o,p);
                         flag=1;
                         give o ~workflag; k--;
                         if (o has supporter && child(o)~=0) SayWhatsOn(o);
                     }
                 }
             }
         }
         else
             if (o has supporter && child(o)~=0) SayWhatsOn(o);
      }

  if (k==0) return 0;

  if (text1~=0)
  {   new_line;
      if (flag==1) text1=text2;
      print (string) text1, " ";
      WriteListFrom(child(descin),
          ENGLISH_BIT + WORKFLAG_BIT + RECURSE_BIT
          + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT);
      return k;
  }
           
  if (flag==1) L__M(##Look,5,descin); else L__M(##Look,6,descin);
];

! ----------------------------------------------------------------------------
!   Looking.  LookSub(1) is allowed to abbreviate long descriptions, but
!     LookSub(0) (which is what happens when the Look action is generated)
!     isn't.  (Except that these are over-ridden by the player-set lookmode.)
! ----------------------------------------------------------------------------

[ LMode1Sub; lookmode=1; print (string) Story; L__M(##LMode1); ];  ! Brief

[ LMode2Sub; lookmode=2; print (string) Story; L__M(##LMode2); ];  ! Verbose

[ LMode3Sub; lookmode=3; print (string) Story; L__M(##LMode3); ];  ! Superbrief

[ NoteArrival descin;
  descin=location;
  if (descin~=lastdesc)
  {   if (descin.initial~=0) PrintOrRun(descin, initial);
      NewRoom();
      MoveFloatingObjects();
      lastdesc=descin;
  }
];

[ ScoreArrival;
  if (location hasnt visited)
  {   give location visited;
      if (location has scored)
      {   score=score+ROOM_SCORE;
          places_score=places_score+ROOM_SCORE;
      }
  }
];

[ LookSub allow_abbrev  i descin;
  if (parent(player)==0) return RunTimeError(10);
  NoteArrival();
  new_line;
#IFV5; style bold; #ENDIF;
  PrintShortName(location);
#IFV5; style roman; #ENDIF;
  i=parent(player);
  if (location~=thedark && i~=location)
  {   if (i has supporter) { print " ("; L__M(##Look,1); print " "; }
      else { print " ("; L__M(##Look,2); print " "; }
      DefArt(i); print ")"; descin=i;
  }
  if (print_player_flag==1) { print " ("; L__M(##Look,3);
      print " "; @print_obj player; print ")"; }
  new_line;

  if (lookmode<3)
  {   if ((allow_abbrev~=1) || (lookmode==2) || (location hasnt visited))
      {   if (location.describe~=NULL) RunRoutines(location,describe);
          else
          {   if (location.description==0) RunTimeError(11,location);
              else PrintOrRun(location,description);
          }
      }
  }

  if (descin~=location) Locale(location);
  Locale(descin);

  LookRoutine();
  ScoreArrival();

  action=##Look;
  if (AfterRoutines()==1) rtrue;
];

[ ExamineSub i;
  if (location==thedark) return L__M(##Examine,1);
  i=noun.description;
  if (i==0)
  {   if (noun has container) <<Search noun>>;
      if (noun has switchable) { L__M(##Examine,3,noun); rfalse; }
      return L__M(##Examine,2,noun);
  }
  PrintOrRun(noun, description);
  if (noun has switchable) L__M(##Examine,3,noun);
  if (AfterRoutines()==1) rtrue;
];

[ LookUnderSub;
  if (location==thedark) return L__M(##LookUnder,1);
  L__M(##LookUnder,2);
];

[ SearchSub i f;
  if (location==thedark) return L__M(##Search,1);
  objectloop (i in noun) if (i hasnt concealed) f=1;
  if (noun has supporter)
  {   if (f==0) return L__M(##Search,2,noun);
      return L__M(##Search,3,noun);
  }
  if (noun hasnt container) return L__M(##Search,4);
  if (noun hasnt transparent && noun hasnt open)
      return L__M(##Search,5);
  if (AfterRoutines()==1) rtrue;

  i=children(noun);
  if (f==0) return L__M(##Search,6,noun);
  L__M(##Search,7,noun);
];

! ----------------------------------------------------------------------------
!   Verbs which change the state of objects without moving them
! ----------------------------------------------------------------------------

[ UnlockSub;
  if (noun hasnt lockable) return L__M(##Unlock,1);
  if (noun hasnt locked)   return L__M(##Unlock,2);
  if (noun.with_key~=second) return L__M(##Unlock,3);
  give noun ~locked;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Unlock,4,noun);
];

[ LockSub;
  if (noun hasnt lockable) return L__M(##Lock,1);
  if (noun has locked)     return L__M(##Lock,2);
  if (noun has open)       return L__M(##Lock,3);
  if (noun.with_key~=second) return L__M(##Lock,4);
  give noun locked;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Lock,5,noun);
];

[ SwitchonSub;
  if (noun hasnt switchable) return L__M(##SwitchOn,1);
  if (noun has on) return L__M(##SwitchOn,2);
  give noun on;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##SwitchOn,3,noun);
];

[ SwitchoffSub;
  if (noun hasnt switchable) return L__M(##SwitchOff,1);
  if (noun hasnt on) return L__M(##SwitchOff,2);
  give noun ~on;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##SwitchOff,3,noun);
];

[ OpenSub;
  if (noun hasnt openable) return L__M(##Open,1);
  if (noun has locked)     return L__M(##Open,2);
  if (noun has open)       return L__M(##Open,3);
  give noun open;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  if (noun has container && noun hasnt transparent && child(noun)~=0)
      return L__M(##Open,4,noun);
  L__M(##Open,5,noun);
];

[ CloseSub;
  if (noun hasnt openable) return L__M(##Close,1);
  if (noun hasnt open)     return L__M(##Close,2);
  give noun ~open;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Close,3,noun);
];

[ DisrobeSub;
  if (noun hasnt worn) return L__M(##Disrobe,1);
  give noun ~worn;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Disrobe,2,noun);
];

[ WearSub;
  if (noun hasnt clothing)  return L__M(##Wear,1);
  if (parent(noun)~=player) return L__M(##Wear,2);
  if (noun has worn)        return L__M(##Wear,3);
  give noun worn;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Wear,4,noun);
];

[ EatSub;
  if (noun hasnt edible) return L__M(##Eat,1);
  remove noun;
  if (AfterRoutines()==1) rtrue;
  if (keep_silent==1) rtrue;
  L__M(##Eat,2,noun);
];

! ----------------------------------------------------------------------------
!   Verbs which are really just stubs (anything which happens for these
!   actions must happen in before rules)
! ----------------------------------------------------------------------------

[ YesSub; L__M(##Yes); ];
[ NoSub; L__M(##No); ];
[ BurnSub; L__M(##Burn); ];
[ PraySub; L__M(##Pray); ];
[ WakeSub; L__M(##Wake); ];
[ WakeOtherSub;
  if (RunLife(noun,##WakeOther)~=0) rfalse;
  L__M(##WakeOther);
];
[ ThinkSub; L__M(##Think); ];
[ SmellSub; L__M(##Smell); ];
[ ListenSub; L__M(##Listen); ];
[ TasteSub; L__M(##Taste); ];
[ DigSub; L__M(##Dig); ];
[ CutSub; L__M(##Cut); ];
[ JumpSub; L__M(##Jump); ];
[ JumpOverSub; L__M(##JumpOver); ];
[ TieSub; L__M(##Tie); ];
[ DrinkSub; L__M(##Drink); ];
[ FillSub; L__M(##Fill); ];
[ SorrySub; L__M(##Sorry); ];
[ StrongSub; L__M(##Strong); ];
[ MildSub; L__M(##Mild); ];
[ SwimSub; L__M(##Swim); ];
[ SwingSub; L__M(##Swing); ];
[ BlowSub; L__M(##Blow); ];
[ RubSub; L__M(##Rub); ];
[ SetSub; L__M(##Set); ];
[ SetToSub; L__M(##SetTo); ];
[ WaveHandsSub; L__M(##WaveHands); ];
[ BuySub; L__M(##Buy); ];
[ SingSub; L__M(##Sing); ];
[ ClimbSub; L__M(##Climb); ];
[ SleepSub; L__M(##Sleep); ];
[ ConsultSub; L__M(##Consult,1,noun); ];
[ TouchSub;
  if (noun==player) return L__M(##Touch,3);
  if (noun has animate) return L__M(##Touch,1);
  L__M(##Touch,2); ];
[ WaveSub;
  if (parent(noun)~=player) return L__M(##Wave,1);
  L__M(##Wave,2,noun); ];
[ PullSub;
  if (noun has static)   return L__M(##Pull,1);
  if (noun has scenery)  return L__M(##Pull,2);
  if (noun has animate)  return L__M(##Pull,4);
  L__M(##Pull,3);
];
[ PushSub;
  if (noun has static)   return L__M(##Push,1);
  if (noun has scenery)  return L__M(##Push,2);
  if (noun has animate)  return L__M(##Pull,4);
  L__M(##Push,3);
];
[ TurnSub;
  if (noun has static)   return L__M(##Turn,1);
  if (noun has scenery)  return L__M(##Turn,2);
  if (noun has animate)  return L__M(##Pull,4);
  L__M(##Turn,3);
];

[ WaitSub;
  if (AfterRoutines()==1) rtrue;
  L__M(##Wait);
];

[ PushDirSub; L__M(##PushDir,1); ];
[ AllowPushDir i;
  if (parent(second)~=compass) return L__M(##PushDir,2);
  if (second==u_obj or d_obj)  return L__M(##PushDir,3);
  AfterRoutines(); i=noun; move i to player;
  <Go second>;
  if (location==thedark) move i to real_location;
  else move i to location;
];

[ SqueezeSub;
  if (noun has animate) return L__M(##Squeeze,1);
  L__M(##Squeeze,2);
];

[ ThrowAtSub;
  if (second>1)
  {   action=##ThrownAt;
      if (RunRoutines(second,before)~=0) { action=##ThrowAt; rtrue; }
      action=##ThrowAt;
  }
  if (second hasnt animate) return L__M(##ThrowAt,1);
  if (RunLife(second,##ThrowAt)~=0) rfalse;
  L__M(##ThrowAt,2);
];

[ AttackSub;
  if (noun has animate && RunLife(noun,##Attack)~=0) rfalse;
  L__M(##Attack); ];

[ KissSub;
  if (RunLife(noun,##Kiss)~=0) rfalse;
  if (noun==player) return L__M(##Touch,3);
  L__M(##Kiss);
];

[ AnswerSub;
  if (RunLife(second,##Answer)~=0) rfalse;
  L__M(##Answer);
];  

[ TellSub;
  if (noun==player) return L__M(##Tell);
  if (RunLife(noun,##Tell)~=0) rfalse;
  L__M(##Tell,2);
];  
  
[ AskSub;
  if (RunLife(noun,##Ask)~=0) rfalse;
  L__M(##Ask);
];  

[ AskForSub;
  if (noun==player) <<Inv>>;
  L__M(##Order,1,noun);
];

! ----------------------------------------------------------------------------
!   Debugging verbs
! ----------------------------------------------------------------------------

#IFDEF DEBUG;
[ TraceOnSub; parser_trace=1; "[Trace on.]"; ];
[ TraceLevelSub; parser_trace=noun;
  print "[Parser tracing set to level ", parser_trace, ".]^"; ];
[ TraceOffSub; parser_trace=0; "Trace off."; ];
[ RoutinesOnSub;  debug_flag=debug_flag | 1; "[Routine listing on.]"; ];
[ RoutinesOffSub; debug_flag=debug_flag & 6; "[Routine listing off.]"; ];
[ ActionsOnSub;  debug_flag=debug_flag | 2; "[Action listing on.]"; ];
[ ActionsOffSub; debug_flag=debug_flag & 5; "[Action listing off.]"; ];
[ TimersOnSub;  debug_flag=debug_flag | 4; "[Timers listing on.]"; ];
[ TimersOffSub; debug_flag=debug_flag & 3; "[Timers listing off.]"; ];
[ CommandsOnSub;
  @output_stream 4; xcommsdir=1; "[Command recording on.]"; ];
[ CommandsOffSub;
  if (xcommsdir==1) @output_stream -4;
  xcommsdir=0;
  "[Command recording off.]"; ];
[ CommandsReadSub;
  @input_stream 1; xcommsdir=2; "[Replaying commands.]"; ];
[ PredictableSub i; i=random(-100);
  "[Random number generator now predictable.]"; ];
[ XPurloinSub; move noun to player; give noun moved ~concealed; "[Purloined.]"; ];
[ XAbstractSub; move noun to second; "[Abstracted.]"; ];
[ XObj obj f;
  if (parent(obj)==0) PrintShortName(obj); else InDefArt(obj);
  print " (", obj, ") ";
  if (f==1) { print "(in "; PrintShortName(parent(obj));
              print " ", parent(obj), ")"; }
  new_line;
  if (child(obj)==0) rtrue;
  WriteListFrom(child(obj),
      FULLINV_BIT + INDENT_BIT + NEWLINE_BIT + ALWAYS_BIT, 1);
];
[ XTreeSub i;
  if (noun==0)
  {   for (i=selfobj+1:i<=top_object:i++)
      {   if (parent(i)==0) XObj(i);
      }
      rfalse;
  }
  XObj(noun,1);
];
[ GotoSub;
  if (noun>top_object || noun<=selfobj || parent(noun)~=0)
      "[Not a safe place.]";
  PlayerTo(noun);
];
[ GonearSub x; x=noun; while (parent(x)~=0) x=parent(x); PlayerTo(x); ];
[ Print_ScL obj; print_ret ++x_scope_count, ": ", (a) obj, " (", obj, ")"; ];
[ ScopeSub; x_scope_count=0; LoopOverScope(#r$Print_ScL, noun);
  if (x_scope_count==0) "Nothing is in scope.";
];
#ENDIF;

! ----------------------------------------------------------------------------
!   Finally: virtually all the text produced by library routines, except for
!   some parser errors (which are indirected through ParserError).
! ----------------------------------------------------------------------------

[ L__M act n x1 s;
  s=sw__var; sw__var=act; if (n==0) n=1;
  L___M(n,x1);
  sw__var=s;
];

[ L___M n x1 s;
  s=action;
#IFDEF LibraryMessages;
  lm_n=n; lm_o=x1;
  action=sw__var;
  if (RunRoutines(LibraryMessages,before)~=0) { action=s; rfalse; }
  action=s;
#ENDIF;
  Prompt:  print "^>"; rtrue;
  Miscellany: if (n==1) "(considering the first sixteen objects only)^";
           if (n==2) "Nothing to do!";
           if (n==3) { print " You have died "; rtrue; }
           if (n==4) { print " You have won "; rtrue; }
           if (n==5)
           {   print "^Would you like to RESTART, RESTORE a saved game";
               if (TASKS_PROVIDED==0)
                   print ", give the FULL score for that game";
               if (deadflag==2 && AMUSING_PROVIDED==0)
                   print ", see some suggestions for AMUSING things to do";
               " or QUIT?"; }
           if (n==6) "[Your interpreter does not provide ~undo~.  Sorry!]";
           if (n==7) "~Undo~ failed.  [Not all interpreters provide it.]";
           if (n==8) "Please give one of the answers above.";
           if (n==9) "^It is now pitch dark in here!";
           if (n==10) "I beg your pardon?";
           if (n==11) "[You can't ~undo~ what hasn't been done!]";
           if (n==12) "[Can't ~undo~ twice in succession. Sorry!]";
           if (n==13) "[Previous turn undone.]";
           if (n==14) "Sorry, that can't be corrected.";
           if (n==15) "Think nothing of it.";
           if (n==16) "~Oops~ can only correct a single word.";
  Order:   CDefArt(x1); " has better things to do.";
  Quit:    if (n==1) { print "Please answer yes or no."; rtrue; }
           print "Are you sure you want to quit? "; rtrue;
  Restart: if (n==1) { print "Are you sure you want to restart? "; rtrue; }
           "Failed.";
  Restore: if (n==1) "Restore failed.";
           "Ok.";
  Save:    if (n==1) "Save failed.";
           "Ok.";
  Verify:  if (n==1) "The game file has verified as intact.";
           "The game file did not verify properly, and may be corrupted \
           (or you may be running it on a very primitive interpreter which \
           is unable properly to perform the test).";
  ScriptOn: if (n==1) "Transcripting is already on.";
           "Start of a transcript of";
  ScriptOff: if (n==1) "Transcripting is already off.";
           "^End of transcript.";
  NotifyOn: "Score notification on.";
  NotifyOff: "Score notification off.";
  Places:  print "You have visited: "; rtrue;
  Objects: if (n==1) "Objects you have handled:^";
           "None.";
  Score:   if (deadflag==0) print "You have so far scored ";
           else print "In that game you scored ";
           print score, " out of a possible ", MAX_SCORE,
                 ", in ", turns, " turn";
           if (turns>1) print "s"; rtrue;
  FullScore: if (n==1)
           {   if (deadflag==0) print "The score is ";
               else             print "The score was ";
               "made up as follows:^"; }
           if (n==2) "finding sundry items";
           if (n==3) "visiting various places";
           print "total (out of ", MAX_SCORE; ")";
  Inv:     if (n==1) "You are carrying nothing.";
           print "You are carrying"; rtrue;
  Take:    if (n==1) "Taken.";
           if (n==2) "You are always self-possessed.";
           if (n==3) { print "I don't suppose "; DefArt(x1);
                       " would care for that."; }
           if (n==4) { print "You'd have to get ";
                       if (x1 has supporter) print "off "; else print "out of ";
                       DefArt(x1); " first."; }
           if (n==5) "You already have that.";
           if (n==6) { print "That seems to belong to "; DefArt(x1); "."; }
           if (n==7) { print "That seems to be a part of "; DefArt(x1); "."; }
           if (n==8) "That isn't available.";
           if (n==9) { CDefArt(x1); " is not open."; }
           if (n==10) "That's hardly portable.";
           if (n==11) "That's fixed in place.";
           if (n==12) "You're carrying too many things already.";
           print "(putting "; DefArt(x1); print " into ";
           DefArt(SACK_OBJECT); " to make room)";
  Drop:    if (n==1) "Already on the floor.";
           if (n==2) "You haven't got that.";
           if (n==3) { print "(first taking "; DefArt(x1); " off)"; }
           "Dropped.";
  Remove:  if (n==1) "It is unfortunately closed.";
           if (n==2) "But it isn't there now.";
           "Removed.";
  PutOn:   if (n==1) { print "You need to be holding ";
                       DefArt(x1); " before you can put it on top \
                      of something else."; }
           if (n==2) "You can't put something on top of itself.";
           if (n==3) { print "Putting things on "; DefArt(x1);
                       " would achieve nothing."; }
           if (n==4) "You lack the dexterity.";
           if (n==5) "(first taking it off)^";
           if (n==6) { print "There is no more room on "; DefArt(x1); "."; }
           if (n==7) "Done.";
           print "You put "; DefArt(x1); print " on "; DefArt(second); ".";
  Insert:  if (n==1) "You need to be holding it before you can put it into \
                      something else.";
           if (n==2) "That can't contain things.";
           if (n==3) "Alas, it is closed.";
           if (n==4) "You'll need to take it off first.";
           if (n==5) "You can't put something inside itself.";
           if (n==6) "(first taking it off)^";
           if (n==7) { print "There is no more room in "; DefArt(x1); "."; }
           if (n==8) "Done.";
           print "You put "; DefArt(x1); print " into "; DefArt(second); ".";
  Transfer: if (n==1) "That isn't in your possession.";
           "First pick that up.";
  EmptyT:  if (n==1) { CDefArt(x1); " can't contain things."; }
           if (n==2) { CDefArt(x1); " is closed."; }
           CDefArt(x1); " is empty already.";
  Give:    if (n==1) { print "You aren't holding "; DefArt(x1); "."; }
           if (n==2) { print "You juggle "; DefArt(x1);
                       " for a while, but don't achieve much."; }
           CDefArt(x1); " doesn't seem interested.";
  Show:    if (n==1) { print "You aren't holding "; DefArt(x1); "."; }
           CDefArt(x1); " is unimpressed.";
  Enter:   if (n==1) { print "But you're already ";
                       if (x1 has supporter) print "on ";
                       else print "in "; DefArt(x1); "."; }
           if (n==2) "That's not something you can enter.";
           if (n==3) { print "You can't get into the closed ";
                       PrintShortName(x1); "."; }
           if (n==4) "You can only get into something on the floor.";
           print "You get "; if (x1 has supporter) print "onto ";
           else print "into "; DefArt(x1); ".";
  GetOff:  print "But you aren't on "; DefArt(x1); " at the moment.";
  Exit:    if (n==1) "But you aren't in anything at the moment.";
           if (n==2) { print "You can't get out of the closed ";
                       PrintShortName(x1); "."; }
           print "You get "; if (x1 has supporter) print "off ";
           else print "out of "; DefArt(x1); ".";
  VagueGo: "You'll have to say which compass direction to go in.";
  Go:      if (n==1)
           {   print "You'll have to get ";
               if (x1 has supporter) print "off "; else print "out of ";
               DefArt(x1); " first.";
           }
           if (n==2) "You can't go that way.";
           if (n==3) { print "You are unable to climb "; Defart(x1); "."; }
           if (n==4) { print "You are unable to descend "; Defart(x1); "."; }
           if (n==5) { print "You can't, since "; Defart(x1);
                       " is in the way."; }
           print "You can't, since "; DefArt(x1); " leads nowhere.";
  LMode1:  " is now in its normal ~brief~ printing mode, which gives \
           long descriptions of places never before visited and short \
           descriptions otherwise.";
  LMode2:  " is now in its ~verbose~ mode, which always gives long \
           descriptions of locations (even if you've been there before).";
  LMode3:  " is now in its ~superbrief~ mode, which always gives short \
           descriptions of locations (even if you haven't been there before).";
  Look:    if (n==1) { print "on"; rfalse; }
           if (n==2) { print "in"; rfalse; }
           if (n==3) { print "as"; rfalse; }
           if (n==4)
           {   print "^On "; DefArt(x1);
               WriteListFrom(child(x1),
                   ENGLISH_BIT + RECURSE_BIT + PARTINV_BIT
                   + TERSE_BIT + ISARE_BIT + CONCEAL_BIT);
               ".";
           }
           if (x1~=location)
           {   if (x1 has supporter) print "^On "; else print "^In ";
               DefArt(x1); print " you";
           }
           else print "^You";
           print " can "; if (n==5) print "also "; print "see ";
           WriteListFrom(child(x1),
                ENGLISH_BIT + WORKFLAG_BIT + RECURSE_BIT
                + PARTINV_BIT + TERSE_BIT + CONCEAL_BIT);
           if (x1~=location) ".";
           " here.";
  Examine: if (n==1) "Darkness, noun.  An absence of light to see by.";
           if (n==2) { print "You see nothing special about ";
                       Defart(x1); "."; }
           CDefArt(x1); print " is currently switched ";
           if (x1 has on) "on."; else "off.";
  LookUnder: if (n==1) "But it's dark.";
           "You find nothing of interest.";
  Search:  if (n==1) "But it's dark.";
           if (n==2) { print "There is nothing on "; DefArt(x1); "."; }
           if (n==3)
           {   print "On "; DefArt(x1);
               WriteListFrom(child(x1),
                   TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
               ".";
           }
           if (n==4) "You find nothing of interest.";
           if (n==5) "You can't see inside, since it is closed.";
           if (n==6) { CDefArt(x1); " is empty."; }
           print "In "; DefArt(x1);
           WriteListFrom(child(x1),
               TERSE_BIT + ENGLISH_BIT + ISARE_BIT + CONCEAL_BIT);
           ".";
  Unlock:  if (n==1) "That doesn't seem to be something you can unlock.";
           if (n==2) "It's unlocked at the moment.";
           if (n==3) "That doesn't seem to fit the lock.";
           print "You unlock "; DefArt(x1); ".";
  Lock:    if (n==1) "That doesn't seem to be something you can lock.";
           if (n==2) "It's locked at the moment.";
           if (n==3) "First you'll have to close it.";
           if (n==4) "That doesn't seem to fit the lock.";
           print "You lock "; DefArt(x1); ".";
  SwitchOn: if (n==1) "That's not something you can switch.";
           if (n==2) "That's already on.";
           print "You switch "; DefArt(x1); " on.";
  SwitchOff: if (n==1) "That's not something you can switch.";
           if (n==2) "That's already off.";
           print "You switch "; DefArt(x1); " off.";
  Open:    if (n==1) "That's not something you can open.";
           if (n==2) "It seems to be locked.";
           if (n==3) "It's already open.";
           if (n==4)
           {   print "You open "; DefArt(x1); print ", revealing ";
               if (WriteListFrom(child(x1),
                   ENGLISH_BIT + TERSE_BIT + CONCEAL_BIT)==0) "nothing.";
               ".";
           }
           print "You open "; DefArt(x1); ".";
  Close:   if (n==1) "That's not something you can close.";
           if (n==2) "It's already closed.";
           print "You close "; DefArt(x1); ".";
  Disrobe: if (n==1) "You're not wearing that.";
           print "You take off "; DefArt(x1); ".";
  Wear:    if (n==1) "You can't wear that!";
           if (n==2) "You're not holding that!";
           if (n==3) "You're already wearing that!";
           print "You put on "; DefArt(x1); ".";
  Eat:     if (n==1) "That's plainly inedible.";
           print "You eat "; DefArt(x1); ".  Not bad.";
  Yes, No: "That was a rhetorical question.";
  Burn:    "This dangerous act would achieve little.";
  Pray:    "Nothing practical results from your prayer.";
  Wake:    "The dreadful truth is, this is not a dream.";
  WakeOther: "That seems unnecessary.";
  Kiss:    "Keep your mind on the game.";
  Think:   "What a good idea.";
  Smell:   "You smell nothing unexpected.";
  Listen:  "You hear nothing unexpected.";
  Taste:   "You taste nothing unexpected.";
  Touch:   if (n==1) "Keep your hands to yourself!";
           if (n==3) "If you think that'll help.";
           "You feel nothing unexpected.";
  Dig:     "Digging would achieve nothing here.";
  Cut:     "Cutting that up would achieve little.";
  Jump:    "You jump on the spot, fruitlessly.";
  JumpOver, Tie: "You would achieve nothing by this.";
  Drink:   "There's nothing suitable to drink here.";
  Fill:    "But there's no water here to carry.";
  Sorry:   "Oh, don't apologise.";
  Strong:  "Real adventurers do not use such language.";
  Mild:    "Quite.";
  Attack:  "Violence isn't the answer to this one.";
  Swim:    "There's not enough water to swim in.";
  Swing:   "There's nothing sensible to swing here.";
  Blow:    "You can't usefully blow that.";
  Rub:     "You achieve nothing by this.";
  Set:     "No, you can't set that.";
  SetTo:   "No, you can't set that to anything.";
  WaveHands: "You wave, feeling foolish.";
  Wave:    if (n==1) "But you aren't holding that.";
           print "You look ridiculous waving "; DefArt(x1); ".";
  Pull, Push, Turn: if (n==1) "It is fixed in place.";
           if (n==2) "You are unable to.";
           if (n==4) "That would be less than courteous.";
           "Nothing obvious happens.";
  PushDir: if (n==1) "Is that the best you can think of?";
           if (n==2) "That's not a direction.";
           "Not that way you can't.";
  Squeeze: if (n==1) "Keep your hands to yourself.";
           "You achieve nothing by this.";
  ThrowAt: if (n==1) "Futile.";
           "You lack the nerve when it comes to the crucial moment.";
  Tell:    if (n==1) "You talk to yourself a while.";
           "This provokes no reaction.";
  Answer, Ask:  "There is no reply.";
  Buy:     "Nothing is on sale.";
  Sing:    "Your singing is abominable.";
  Climb:   "I don't think much is to be achieved by that.";
  Wait:    "Time passes.";
  Sleep:   "You aren't feeling especially drowsy.";
  Consult: print "You discover nothing of interest in "; DefArt(x1); ".";
];

! ----------------------------------------------------------------------------
