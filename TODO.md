# TODO

## Features
- [ ] Print upgrades
- [ ] Print upgrade costs
- [ ] Possibly add images
- [ ] GUI??
- [ ] Uh, try to update it more often. Missing the tier lists happens too easily...

## Bugs

## Organisation/documentation

- [ ] Continue improving Readme and other repository files!
- [ ] Add message about changing files if you want to
  - [ ] Troubleshooting exceptions
  - [ ] Adding newly released heroes and changing/updating tier lists
- [ ] Figure out handling run directory/data file paths better if possible
- [ ] Add code documentation e.g. change comments to Doxygen style

## Distribution/installer
- [ ] Think about adding GitHub Actions for executable build?
- [ ] Linux package??
- [ ] Add updater to installer, and add msg about data files to updater (they will be overwritten, so make sure to back them up if you make changes you want to keep)
- [ ] Improve installer UI and features (what do people do when they make real installers?)
- [ ] Choose install location (warn against Program Files since permissions are needed). 
- [ ] owned.csv doesn't get removed by uninstaller. 
- [ ] Please install in the same location if you're reinstalling.

# BACKLOG

# DONE
- [x] Add more checks and throw more exceptions
  - [x] Program fails silently if data files aren't found
  - [x] Mistakes are possible if data files are invalid (missing or wrong fields, hero names that don't match the heroes data file)
- [x] Add a higher level of the menu
  - [x] Filter
  - [x] Validate data files
- [x] Figure out distributing executables better if possible
- [x] Data files aren't found when you run the program in the wrong directory
- [x] Think about alternatives to .csv for data files maybe?
  - I thought about it
- [x] Use this definition/standard: https://www.ietf.org/rfc/rfc4180.txt (section 2) for CSV (urgent!)
- [x] Thor's name having a comma means the code is broken until I change data files and the processing code.
- [x] Change/add owned heroes (urgent!)
