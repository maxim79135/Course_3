��
TCOLS V1.1  Copr.1994 Eric Isaacson, 416 E. Univ.St., BMG IN 47401
  Permission to use granted only to registered A86 users.

TCOLS converts single-column input into paged, multi-column output.
Usage: TCOLS <in >out #1 #2 #3 #4 #5          , where
  #1 is the number of major columns you want each page split into
  #2 is the number of lines to skip between each page (default 6)
  #3 is the number of chars per line the printer is now set to (default 80)
  #4 is the number of lines per page the printer is now set to (default 66)
  #5 is which line within the first page the printer is now at (default 0)

Examples:
TCOLS <MYPROG.XRF >PRN 4 6 96 88
   sends the file MYPROG.XRF to the printer, split into 4 columns, where
   the printer is set to 96 characters a line, 8 lines per inch.
TCOLS <NARROW.LST 6 0 80 23 22 | MORE
   provides 6-column screen-paged output.
NOTE for readability, TCOLS will convert underscores to hyphens when the
   lines per page is greater than 80.
�W �t� ��F��u���P�< v�N<�t,0<
sXú�����*�v�*�v����� ���ᑿ�� �þ� ���� ���t��� �姓�� �맰P� ����*̴ ��<r��ȣ秉���駰B� �槰 � ��������������% t�� r����;�r�.㧋�;.�s��Ë㧬< r<�s<_t:���NN��&�E� �<
u���t� u��SQW�
���}�uA��_� �Y)�[빀>�Pv��-뻴 ���t�SR+۬,0�
 8�s�������N�Z[�QRV��F�秾����� � ���^ZY�&맄�t�� ��u��QV��W���O� ���tA_�^YP�
�X���fr�PSQR� ��F��)��( ��ZY[X�PSQR+ۺ�g� @� ���� ���ZY[Xô?�!ô@��� �L�� ������