/*************************************/
    /* Name: Oomat Latipov */
    /* NetID: U13921223 */
    /* Program Description: This program */
    /* that reads a csv file and */
    /* finds the monthly weather statistics. */
/*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definition of maximum amount of days and months.
#define MAX_MONTHS 12
#define MAX_DAYS 31

int main(int argc, char *argv[]) {
    // Checks if the number of arguments and if it is wrong exits the program.
    if (argc != 2) {
        printf("Usage: %s <csv_filename>\n", argv[0]);
        return 1;
    }

    // Input CSV file
    char *input_file_name = argv[1];
    FILE *fp = fopen(input_file_name, "r");

    // Checking if everything is alright with the file.
    if (!fp) {
        printf("Can't open file %s\n", input_file_name);
        return 1;
    }

    // Output file name
    // Declares a character array and appends ".txt" into the file name.
    char output_file_name[256];
    snprintf(output_file_name, sizeof(output_file_name), "%s.txt", input_file_name);

    // Opens the output file name and checks if it can be opened.
    FILE *fout = fopen(output_file_name, "w");
    if (!fout) {
        printf("Can't open output file %s\n", output_file_name);
        fclose(fp);
        return 1;
    }

    // Declaring variables for future use.
    char buffer[1024];
    float temperature_sum[MAX_MONTHS] = {0.0};
    int day_count[MAX_MONTHS] = {0};
    int used_days[MAX_MONTHS][MAX_DAYS] = {0};  // 2D array to keep track of used days.
    int row = 0;

    // Reads each line from the input file and skips the header line.
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (row == 0) {  // Skip header line.
            row++;
            continue;
        }

        // Declaring essential variables.
        int month, day;
        float temperature;

        // Parses the line to extract month, day, and temperature.
        if (sscanf(buffer, "%d,%d,%f", &month, &day, &temperature) != 3) {
            continue;  // Skip lines that don't match the format.
        }

        // Checks if the month or day values are invalid.
        if (month < 1 || month > 12 || day < 1 || day > 31) {
            continue;  // Ignore invalid data.
        }

        if (!used_days[month - 1][day - 1]) {  // Check if the day has already been used.
            temperature_sum[month - 1] += temperature;
            day_count[month - 1]++;
            used_days[month - 1][day - 1] = 1;  // Mark the day as used.
        }
    }

    // Write the statistics to the output file.
    const char *month_names[MAX_MONTHS] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    // Declaring variables for total calculations.
    int total_days = 0;
    float total_temperature = 0.0;
    int measured_months = 0;
    
    // Iterates over each month cahnging the number of months, days and temperatures.
    for (int i = 0; i < MAX_MONTHS; i++) {
        if (day_count[i] > 0) {
            measured_months++;
            total_days += day_count[i];
            total_temperature += temperature_sum[i];

            // Prints out all info.
            fprintf(fout, "***** %s *****\n", month_names[i]);
            fprintf(fout, "Measured days: %d\n", day_count[i]);
            fprintf(fout, "Average temperature: %.2f\n\n", temperature_sum[i] / day_count[i]);
        }
    }

    // Write the total summary to the output file.
    fprintf(fout, "***** Total *****\n");
    fprintf(fout, "Measured months: %d\n", measured_months);
    fprintf(fout, "Measured days: %d\n", total_days);
    fprintf(fout, "Average temperature: %.2f\n\n", total_temperature / total_days);

    // Closes the files.
    fclose(fp);
    fclose(fout);

    return 0;
}
