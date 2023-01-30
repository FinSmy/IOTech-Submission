#include <json-c/json.h>
#include <stdio.h>
#include <time.h>

int main() {
    // Open file 
    FILE *jsonFile = fopen("/.exercise-02/data/data.json", "r");
    // Create json object from file
    struct json_object *root = json_object_from_file(jsonFile);
    fclose(jsonFile);

    // Get the Devices array
    struct json_object *devicesArray;
    json_object_object_get_ex(root, "Devices", &devicesArray);

    int total = 0, uuidCount = 0;
    struct json_object *uuids = json_object_new_array();

    // Iterate through each device in the Devices array
    for (int i = 0; i < json_object_array_length(devicesArray); i++) {
        struct json_object *device = json_object_array_get_idx(devicesArray, i);
        // Get timestamp value
        int timestamp;
        json_object_object_get_ex(device, "timestamp", &timestamp);

        // Compare timestamp to the current time
        time_t currentTime = time(NULL);
        if (timestamp < currentTime) {
            continue;
        }

        // Get value
        struct json_object *value;
        json_object_object_get_ex(device, "value", &value);
        //Convert to int and add to total
        total += atoi(json_object_get_string(value));

        // Get Info object
        struct json_object *info;
        json_object_object_get_ex(device, "Info", &info);
        // Extract uuid value from info string
        char *infoString = (char *)json_object_get_string(info);
        char *uuid = strstr(infoString, "uuid:") + 5;
        // Add uuid to list of uuids
        json_object_array_add(uuids, json_object_new_string(uuid));

        uuidCount++;
    }

    // Create json object to store total and uids
    struct json_object *output = json_object_new_object();
    json_object_object_add(output, "total", json_object_new_int(total));
    json_object_object_add(output, "uuids", uuids);

    // Convert the output json object to a string
    char *outputString = (char *)json_object_to_json_string(output);

    // Write the output string to a file
    FILE *outputFile = fopen("output.json", "w");
    fwrite(outputString, sizeof(char), strlen(outputString), outputFile);
    fclose;
}