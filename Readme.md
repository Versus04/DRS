# F1 Elo Rating Engine

A comprehensive Formula 1 Elo rating system that analyzes driver performance across F1 history (1950-2024) using race results data to calculate both current and all-time peak ratings.

## Overview

This engine implements a sophisticated Elo rating system specifically designed for Formula 1 racing, taking into account:
- Position-based comparisons between drivers
- Era-specific adjustments for different F1 periods
- Seasonal activity tracking and inactivity penalties
- Performance bonuses based on finishing positions

## Features

- **Historical Data Processing**: Processes F1 data from 1950 to 2024
- **Dynamic K-Factor**: Adjusts rating changes based on race importance and era
- **Peak Rating Tracking**: Maintains both current and all-time peak ratings
- **Inactivity Penalties**: Reduces ratings for inactive drivers between seasons
- **Era Adjustments**: Different rating multipliers for various F1 eras
- **Position Bonuses**: Additional points based on F1 points system

## Data Requirements

The engine expects three CSV files in a `dataset/` directory:

### `drivers.csv`
Contains driver information with columns:
- `driverId` - Unique driver identifier
- `driverRef` - Driver reference code
- `forename` - Driver's first name
- `surname` - Driver's last name
- Additional metadata fields

### `races.csv`
Contains race information with columns:
- `raceId` - Unique race identifier
- `year` - Race year
- `round` - Round number in season
- `circuitId` - Circuit identifier
- `name` - Race name
- `date` - Race date

### `results.csv`
Contains race results with columns:
- `raceId` - Reference to race
- `driverId` - Reference to driver
- `positionOrder` - Final position (1st, 2nd, etc.)
- `statusId` - Race finish status

## Rating System Details

### Base Elo Formula
```
New Rating = Old Rating + K × (Actual Score - Expected Score)
```

### Dynamic K-Factor
- **Base K-Factor**: 15.0
- **Race Winner**: 25.0 (increased importance)
- **Pre-1980 Era**: ×0.8 (historical adjustment)
- **2014-2020 Era**: ×1.2 (hybrid era emphasis)

### Position Bonuses
Based on F1 points system:
- 1st: 25 points ÷ 5 = 5 rating points
- 2nd: 18 points ÷ 5 = 3.6 rating points
- 3rd: 15 points ÷ 5 = 3 rating points
- ... (decreasing to 10th place)

### Inactivity Penalty
- Drivers not racing in a season lose 10 rating points
- Minimum rating floor of 1500 to prevent excessive degradation

## Compilation and Usage

### Prerequisites
- C++ compiler with C++11 support (g++, clang++, MSVC)
- F1 dataset CSV files

### Build Instructions

**Windows (PowerShell):**
```powershell
g++ -o f1elo main.cpp
```

**Linux/Mac:**
```bash
g++ -o f1elo main.cpp
```

### Running the Engine
```bash
./f1elo
```

## Output

The program generates two leaderboards:

1. **Top 20 Current Elo Ratings**: Current ratings as of the latest season
2. **Top 20 All-Time Peak Ratings**: Highest ratings ever achieved by each driver

Example output:
```
Top 20 Elo current Ratings (1950-2024)
Lewis Hamilton -> 1847.32
Max Verstappen -> 1823.45
...

Top 20 Elo All Time Ratings (1950-2024)  
Michael Schumacher -> 1923.67
Ayrton Senna -> 1891.23
...
```

## File Structure

```
F1-Elo-Engine/
├── main.cpp          # Main engine logic
├── driver.h          # Driver structure definition
├── race.h            # Race structure definition  
├── raceresults.h     # Race result structure definition
├── dataset/          # CSV data files
│   ├── drivers.csv
│   ├── races.csv
│   └── results.csv
└── README.md
```

## Algorithm Highlights

1. **Chronological Processing**: Races are sorted by year and round to ensure proper temporal order
2. **Pairwise Comparisons**: Every driver is compared against every other driver in each race
3. **Era Sensitivity**: Different eras receive different K-factor multipliers
4. **Activity Tracking**: Seasonal analysis identifies active vs inactive drivers
5. **Peak Preservation**: Tracks the highest rating each driver has ever achieved

## Technical Details

- **Language**: C++11
- **Data Structures**: Hash maps for O(1) lookups, vectors for ordered processing
- **Memory Efficiency**: Streams data from CSV files without loading everything into memory
- **Error Handling**: Basic CSV parsing with quote handling

## Customization

Key parameters that can be adjusted:

```cpp
// In main.cpp
const double DEFAULT_RATING = 1500.0;     // Starting rating
const double BASE_K_FACTOR = 15.0;        // Base rating change rate
const double WINNER_K_FACTOR = 25.0;      // Winner bonus multiplier
const double INACTIVITY_PENALTY = 10.0;   // Yearly inactivity penalty
```

## Data Source

This engine is designed to work with the [Ergast F1 Database](http://ergast.com/mrd/) CSV export format.

## License

Open source project - feel free to modify and distribute.

## Contributing

Contributions welcome! Areas for improvement:
- Additional error handling
- Configuration file support
- More sophisticated era adjustments
- Team-based ratings
- Qualifying performance integration

---

*Built for F1 statistical analysis and historical driver comparison.*