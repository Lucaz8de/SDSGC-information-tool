# TODO

## Features
- [ ] Ability to print upgrades (might as well review existing print too)
- [ ] Print upgrades when updating an already owned hero
- [ ] Print upgrade costs
- [ ] Think about adding true awakening, costumes/holy relics to upgrades
- [ ] Possibly add images
- [ ] GUI??
- [ ] Uh, try to update it more often. Missing the tier lists happens too easily...
- [ ] Add quit to the menu; is there anything it would be good to take care of when/before closing?
- [ ] Adding owned heroes needs to be improved somehow - it's totally unreasonable.

## Organisation

## Bugs
- [ ] Need to add error checking for heroes.csv and owned.csv order. They're required to be the same.

## Documentation
- [ ] Continue improving Readme and other repository files!
- [ ] Add message about changing files if you want to
  - [ ] Troubleshooting exceptions
  - [ ] Adding newly released heroes and changing/updating tier lists
- [ ] Figure out handling run directory/data file paths better if possible
- [ ] Add code documentation e.g. change comments to Doxygen style
- [ ] Think about organisation decision that could fix the need to call HashKeys(ReadLists()) in Filter::GetArguments()

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
- [x] Think about adding has LR to hero data
- [x] The organisation of the hero constructor is reeeally ugly. It looks through the whole map every time, when they're literally in order? Do something about that...!
