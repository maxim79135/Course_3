rem This file creates THUMBS.HTM thumbnail files and _SLIDES_.SLS listings
rem   in this and all subdirectories containing GIF and/or JPG files.
rem   See NESTCHEK.8 for details of use.
cd >jth-this
lineplat c:\a86\eitools\thumhead.lpt jth-this jth-titl
dir /b/ad/on >jth-dirs
type jth-dirs
lineplat c:\a86\eitools\thumsub.lpt jth-dirs jth-dLnk
dir /b/on *.jpg >_slides_.sls
dir /b/on *.gif >>_slides_.sls
lineplat c:\a86\eitools\thumlink.lpt _slides_.sls jth-jLnk
cat jth-titl jth-dLnk jth-jLnk >Thumbs.htm
lineplat c:\a86\eitools\thumnest.lpt jth-dirs jth-nest.bat
type jth-nest.bat
call jth-nest.bat
nestchek Thumbs.htm jth-dirs _Slides_.sls
del jth-*.*


