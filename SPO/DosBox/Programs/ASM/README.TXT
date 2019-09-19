A86 assembler and D86 debugger, V4.05       January 14, 2000
Copyright 1986--2000 Eric Isaacson.
All rights reserved.

Eric Isaacson Software       Visa/MC/Amex accepted
416 E. University Ave.       1-812-339-1811 voice
Bloomington, IN 47401-4739   1-812-335-1611 fax
                             eric@eji.com
                             http://eji.com/a86

See Chapter 1 of the manual, A86MANU.TXT, for
permissions/restrictions.

This package is a complete assembly-language compiler
for Intel-x86 and compatible computers, running under
a Microsoft or Microsoft-compatible operating system,
together with a complete manual.  Please upload it to 
Internet Web sites, and give it to user groups, computer 
departments in schools, and anyone else you think might be 
interested in writing assembly-language programs for Intel 86-
family microprocessors. 

You need to read the manual to use this program -- it's 
just too complex to be self-explanatory.  Chapter 2 of the
manual (A86MANU.TXT) takes you through a trial demonstration. 


What's New

V4.05 is a bug-fix release.

V4.04 is the official release of A386 and D386, with a new
release of A86 and D86 with the same version number.  I
have upgraded A386/D386 support for the Pentium 3 processor
to conform to the now-published Intel specifications, and
I have added display windows to D386 for the new SIMD
registers.  I have also added the new AMD Athlon instructions
to A386 assembly and D386 disassembly.

V4.03, V4.02 and V4.01 are bug-fix releases.

V4.00 is the "official release" of A86 with major new features,
although I made an "unofficial release" (versions V3.7x) with
these features earlier.

Let's cover the INCOMPATIBILITIES first, so you will know
where to tweak your existing code: 

* Error messages in A86 now have a slightly different format (on 
  their own line with a pointer to the error location). If you 
  have any source code lying around with old-format error 
  messages, get rid of the messages before reassembling with 
  this new version -- this version cannot handle them. 

* I have changed the L switch to the G (code Generation) switch 
  to prepare for L becoming a Listing-control switch. I have 
  also changed the X switch to +G16, to prepare for X becoming 
  the XRF control switch.  If you have any batch files or A86 
  environment-flag settings with +L or +X in them, change them 
  to +G for this new version. 

* The DATA SEGMENT now has a default ORG value of END; that is, 
  it starts at the end of your program, unless you have an ORG 
  otherwise.  V3.20--V3.22 of A86 outlawed ORGless DATA segments,
  in anticipation of this feature.  Versions before that had a
  default ORG of 0 -- if you have source code written for a 
  pre-V3.20 version of A86, make sure you put ORG 0 at the start
  of the first DATA SEGMENT if it was ORG-less. 

Other new features include an INCLUDE directive (Chapter 9), a 
listing facility (Chapters 3 and 13), END operand value (Chapter 
9), pulling XREF and EXMAC functions into the assembler (Chapter 
13), operand to EVEN (Chapter 9), the removal of the limit on 
the length of a source file, the removal of most restrictions on 
forward-references within expressions, the ability to ORG in the 
DATA segment and structures to a forward-reference value, the 
REF and DEF operators (Chapter 9), the K numeric base (Chapter 
10), numeric operands to STOSx and MOVSx (Chapter 5), and macros 
in an A86LIB library (Chapter 13).

I also have an A386 assembler and D386 debugger, available only
if you register BOTH A86 and D86.  They cover all instructions
up through the Pentium II, including MMX, AMD's 3DNow instruction
set, and the not-yet-released KNI instructions.  Chapter 6 of
the A86 manual describes the new A386 instructions and instruction
formsm, through the Pentium Pro.  The newer instructions are
described in a text file accompanying the A386 package.


How to Get Everything

If you have downloaded the A86Vxxx.ZIP file from a BBS or the
Internet, you need only make a subdirectory on your hard drive, make
that the current subdirectory, then run the program PKUNZIP or
PCUNZIP followed by the name of the ZIP file.  The program will
"unzip" (decompress) all the files of the package into your
subdirectory.


