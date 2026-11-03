# Fuel Delivery Scheduler

A C++ command-line program that plans heating-oil deliveries from a weather forecast.

It reads a multi-day temperature forecast and a list of customers' tanks. For each day, it estimates how much fuel every customer burns and schedules a delivery when a tank runs low. At the end, it reports how much fuel each tank should have left.

## How it works

**1. Heating degree days (HDD).** For each forecast day, the program measures how cold the day is compared with a 65 °F baseline:

```
HDD = max(0, 65 − (high + low) / 2)
```

A day averaging 65 °F or warmer counts as 0 HDD, meaning no heating is needed.

**2. Delivery check.** At the start of each day, any customer whose tank is below **20%** of capacity gets a delivery of **70%** of their tank's capacity.

**3. Fuel usage.** Each customer has a *K-factor*, the gallons they burn per heating degree day. After any delivery, the tank level is updated:

```
new level = old level + delivery − (K-factor × HDD)
```

**4. Report.** The program prints each day's deliveries (or says there are none), followed by the estimated fuel in every tank at the end of the forecast.

## Building

**g++ (Linux / macOS / MinGW):**

```bash
g++ -o fuel_delivery_scheduler fuel_delivery_scheduler.cpp
```

**MSVC (Developer Command Prompt on Windows):**

```bat
cl /EHsc fuel_delivery_scheduler.cpp
```

The program uses only the C++ standard library.

## Usage

Run the program. It will ask for two file paths, one at a time. You can type them or drag the files into the terminal.

```bash
./fuel_delivery_scheduler
```

Sample input files are in [`examples/`](examples/).

## Input file formats

Values are separated by spaces. Text fields can't contain spaces, so use underscores (e.g. `Elm_St`).

### Forecast file

The first line is the number of days. Each line after that is one day: `month day high low`, with temperatures in °F.

```
5
January 10 34 20
January 11 28 15
January 12 40 30
January 13 22 8
January 14 30 18
```

### Customers file

The first line is the number of customers. Each line after that is one customer: `building_number street_name tank_capacity current_fuel k_factor`. The capacity is a whole number of gallons, the current fuel is in gallons, and the K-factor is in gallons per HDD.

```
3
12 Elm_St 275 70.0 0.6
48 Oak_Ave 500 140.0 0.9
7 Maple_Dr 275 200.0 0.4
```

## Example output

Using the sample files above:

```
January 10:
No fuel deliveries scheduled for this day.

January 11:
192.5 gallons -> 12 Elm_St

January 12:
350 gallons -> 48 Oak_Ave

January 13:
No fuel deliveries scheduled for this day.

January 14:
No fuel deliveries scheduled for this day.

ESTIMATED FUEL IN TANK AT THE END OF FORECAST PERIOD

12 Elm_St: 141.0 gallons
48 Oak_Ave: 307.8 gallons
7 Maple_Dr: 119.0 gallons
```

## Project structure

```
fuel_delivery_scheduler.cpp   # all source code
examples/
  forecast.txt                # sample 5-day forecast
  customers.txt               # sample customer list
```

The code is organized into two classes:

- **`ForecastRecord`**: one day's date and high/low temperatures. Computes that day's heating degree days.
- **`CustomerRecord`**: one customer's address, tank size, current fuel level and K-factor. Decides whether the customer needs a delivery and updates their estimated fuel level.

The forecast and customer lists are held in dynamically allocated arrays sized from the first line of each input file.

## Possible improvements

- Allow street names with spaces by reading the rest of each line with `getline`
- Check input files for missing or malformed lines
- Make the delivery threshold, refill amount and 65 °F baseline configurable
- Use `std::vector` instead of manually managed arrays
