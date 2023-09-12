#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <path_to_obj_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open the file");
        return 2;
    }

    char line[256];
    float vertices[15000][3];
    int vertex_count = 0;
    int v1, v2, v3, v4;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            sscanf(line, "v %f %f %f", &vertices[vertex_count][0], &vertices[vertex_count][1], &vertices[vertex_count][2]);
            vertex_count++;
        }
    }

    fseek(file, 0, SEEK_SET);

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'f') {
            if(sscanf(line, "f %d/%*[^ / ]/%*[^ ] %d/%*[^ / ]/%*[^ ] %d/%*[^ / ]/%*[^ ] %d/%*[^ / ]/%*[^ ]", &v1, &v2, &v3, &v4) == 4) {
                // 4-vertex face
                printf("tr %f,%f,%f %f,%f,%f %f,%f,%f 255,255,0 \n",
                    vertices[v1-1][0], vertices[v1-1][1], vertices[v1-1][2],
                    vertices[v2-1][0], vertices[v2-1][1], vertices[v2-1][2],
                    vertices[v3-1][0], vertices[v3-1][1], vertices[v3-1][2]);

                printf("tr %f,%f,%f %f,%f,%f %f,%f,%f 255,255,0 \n",
                    vertices[v1-1][0], vertices[v1-1][1], vertices[v1-1][2],
                    vertices[v3-1][0], vertices[v3-1][1], vertices[v3-1][2],
                    vertices[v4-1][0], vertices[v4-1][1], vertices[v4-1][2]);
            } else {
                sscanf(line, "f %d/%*[^ / ]/%*[^ ] %d/%*[^ / ]/%*[^ ] %d/%*[^ / ]/%*[^ ]", &v1, &v2, &v3);
                printf("tr %f,%f,%f %f,%f,%f %f,%f,%f 255,255,0 \n",
                    vertices[v1-1][0], vertices[v1-1][1], vertices[v1-1][2],
                    vertices[v2-1][0], vertices[v2-1][1], vertices[v2-1][2],
                    vertices[v3-1][0], vertices[v3-1][1], vertices[v3-1][2]);
            }
        }
    }

    fclose(file);
    return 0;
}
