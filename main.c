#include <stdio.h>
#include <time.h>
#include <string.h>

#define MAX_RECORDS_PER_TYPE 100
#define MAX_BUSES 30

struct EntryExitRecord {
    time_t inTime;
    time_t outTime;
    int isActive;//flag
};

struct PrivateVehicleRecord {
    char vehicleNumber[20];
    char reason[100];
    struct EntryExitRecord entryExitRecords[MAX_RECORDS_PER_TYPE];
    int recordCount;
};

struct BusRecord {
    char busNumber[20];
    struct EntryExitRecord entryExitRecords[MAX_RECORDS_PER_TYPE];
    int recordCount;
};
void recordEntry(struct EntryExitRecord records[], int *recordCount) {
    time(&records[*recordCount].inTime);
    records[*recordCount].isActive = 1;
    (*recordCount)++;
}
void recordExit(struct EntryExitRecord records[], int *recordCount) {
    // Record exit details for a vehicle
    time(&records[*recordCount - 1].outTime);
    records[*recordCount - 1].isActive = 0;
    (*recordCount)++;
}
void sendDailyReport(FILE *reportFile, struct PrivateVehicleRecord privateVehicleRecords[], struct BusRecord busRecords[], int privateVehicleCount, int busCount) {
    fprintf(reportFile, "Daily Report\n\n");

    // Report for Private Vehicles
    fprintf(reportFile, "Private Vehicles:\n");
    for (int i = 0; i < privateVehicleCount; ++i) {
        fprintf(reportFile, "Vehicle Number: %s\n", privateVehicleRecords[i].vehicleNumber);
        fprintf(reportFile, "Reason: %s\n", privateVehicleRecords[i].reason);
        for (int j = 0; j < privateVehicleRecords[i].recordCount; ++j) {
            if (privateVehicleRecords[i].entryExitRecords[j].isActive) {
                fprintf(reportFile, "Entry Time: %s\n", asctime(localtime(&privateVehicleRecords[i].entryExitRecords[j].inTime)));
                fprintf(reportFile, "Status: Inside\n\n");
            } else if (privateVehicleRecords[i].entryExitRecords[j].outTime != 0) {
                fprintf(reportFile, "Entry Time: %s\n", asctime(localtime(&privateVehicleRecords[i].entryExitRecords[j].inTime)));
                fprintf(reportFile, "Exit Time: %s\n\n", asctime(localtime(&privateVehicleRecords[i].entryExitRecords[j].outTime)));
            }
        }
    }
    fprintf(reportFile, "\n");
    fprintf(reportFile, "College Vehicles:\n");
    for (int i = 0; i < busCount; ++i) {
        fprintf(reportFile,busRecords[i].busNumber);

        for (int j = 0; j < busRecords[i].recordCount; ++j) {
            if (busRecords[i].entryExitRecords[j].isActive) {
                fprintf(reportFile, "\nEntry Time: %s\n", asctime(localtime(&busRecords[i].entryExitRecords[j].inTime)));
                fprintf(reportFile, "Status: Inside\n\n");
            } else if (busRecords[i].entryExitRecords[j].outTime != 0) {
                fprintf(reportFile, "\nEntry Time: %s\n", asctime(localtime(&busRecords[i].entryExitRecords[j].inTime)));
                fprintf(reportFile, "Exit Time: %s\n\n",
                        asctime(localtime(&busRecords[i].entryExitRecords[j].outTime)));
            }

        }
    }
}

int main() {
    struct PrivateVehicleRecord privateVehicleRecords[MAX_RECORDS_PER_TYPE];
    struct BusRecord busRecords[MAX_BUSES];

    int privateVehicleCount = 0;
    int busCount = 0;
    int ch;
    FILE *reportFile;

    do{
        printf("\n1. Record Entry for Private Vehicles\n");
        printf("2. Record Exit for Private Vehicles\n");
        printf("3. Record Entry for College Bus\n");
        printf("4. Record Exit for College Bus\n");
        printf("5. Generate Daily Report\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        switch(ch) {
            case 1:
                printf("Enter vehicle number: ");
                scanf("%s", privateVehicleRecords[privateVehicleCount].vehicleNumber);
                printf("Enter reason for visit: ");
                getchar();
                fgets(privateVehicleRecords[privateVehicleCount].reason, sizeof(privateVehicleRecords[privateVehicleCount].reason), stdin);
                recordEntry(privateVehicleRecords[privateVehicleCount].entryExitRecords, &privateVehicleRecords[privateVehicleCount].recordCount);
                privateVehicleCount++;
                break;
            case 2:
                if (privateVehicleCount > 0) {
                    printf("Enter vehicle number to exit: ");
                    char exitVehicleNum[20];
                    scanf("%s", exitVehicleNum);
                    int found = 0;
                    for (int i = 0; i < privateVehicleCount; ++i) {
                        if (strcmp(privateVehicleRecords[i].vehicleNumber, exitVehicleNum) == 0) {
                            recordExit(privateVehicleRecords[i].entryExitRecords, &privateVehicleRecords[i].recordCount);
                            found = 1;
                            printf("Data valid");
                            break;
                        }
                    }
                    if (!found) {
                        printf("Vehicle not found in the entered list.\n");
                    }
                } else {
                    printf("No private vehicle entries to exit.\n");
                }
                break;
            case 3:
                printf("Enter bus number: ");
                scanf("%s", busRecords[busCount].busNumber);
                recordEntry(busRecords[busCount].entryExitRecords, &busRecords[busCount].recordCount);
                busCount++;
                break;
            case 4:
                if (busCount > 0) {
                    printf("Enter bus number to exit: ");
                    char exitBusNum[20];
                    scanf("%s", exitBusNum);
                    int found = 0;
                    for (int i = 0; i < busCount; ++i) {
                        if (strcmp(busRecords[i].busNumber, exitBusNum) == 0) {
                            recordExit(busRecords[i].entryExitRecords, &busRecords[i].recordCount);
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        printf("Bus not found in the entered list.\n");
                    }
                } else {
                    printf("No bus entries to exit.\n");
                }
                break;
            case 5:
                reportFile = fopen("daily_report.txt", "a");
                if (reportFile != NULL) {
                    sendDailyReport(reportFile, privateVehicleRecords, busRecords, privateVehicleCount, busCount);
                    fclose(reportFile);
                    printf("Daily report generated successfully.\n");
                } else {
                    printf("Error: Unable to create the report file.\n");
                }
                break;
            case 6:
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");

        }
    } while (ch!=6);

}