rem This will package everything for the tournament distribution together
xcopy ..\client deploy-tournament\client /S /D /I /EXCLUDE:excludes-tournament.txt
xcopy ..\rules.txt deploy-tournament\ /D
xcopy ..\aitournament.ppt deploy-tournament\ /D
xcopy ..\"The story so far.txt" deploy-tournament\ /D