#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

struct Point {
  char name[100];
  double lat;
  double lon;
};

double dis(struct Point A, struct Point B){
	double dlat = (B.lat-A.lat)*M_PI/180.0;
	double dlon = (B.lon-A.lon)*M_PI/180.0;
	double a = pow(sin(dlat/2.0),2) + cos(A.lat*M_PI/180.0) * cos(B.lat*M_PI/180.0) * pow(sin(dlon/2.0),2);
	double b = 2.0 * atan2(sqrt(a), sqrt(1.0-a));
	return 12742.0*b*1000.0;
}

int indDMin(double *d, int taille){
	double x=INT_MAX, ind;
	for(int i=0; i<taille; i++){
		if(d[i]<x){
			x=d[i];
			ind=i;
		}
	}
	return ind;
}

int main() {
  FILE *fp;
  char filename[100];
  char buffer[1000];
  int count = 0;
  struct Point points[300];
  double distance[300];
  printf("Enter GPX filename: ");
  scanf("%s", filename);

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Error: Could not open file %s\n", filename);
    return 1;
  }

  while (fgets(buffer, 1000, fp) != NULL) {
    if(strstr(buffer, "<wpt") !=NULL){
    	char *found_string = strstr(buffer, "lat=");
    	char coord[100];
    	int i=0, j=0;
    	
    	while(found_string[j]!='\"') j++;
    	j++;
    	while(found_string[j]!='\"'){
    		coord[i]=found_string[j];
    		j++;
    		i++;
    	}
    	coord[i]='\0';
    	
    	double test;
    	char *askip;
    	test=strtod(coord, &askip);
    	points[count].lat=test;
    	
    	
    	found_string = strstr(buffer, "lon=");
    	i=0;j=0;
    	coord[0]='\0';
    	while(found_string[j]!='\"') j++;
    	j++;
    	while(found_string[j]!='\"'){
    		coord[i]=found_string[j];
    		j++;
    		i++;
    	}
    	coord[i]='\0';
    	test=strtod(coord, &askip);
    	points[count].lon=test;
    	count++;
    	
    }
  }

  fclose(fp);

  printf("Found %d points\n", count);
  struct Point inconnu;	
  inconnu.lat=42.457; inconnu.lon=-71.1214;
  for (int i = 0; i < count; i++) {
    printf("Point %d: %s (%f, %f)\n", i+1, points[i].name, points[i].lat, points[i].lon);
    distance[i]=dis(points[i], inconnu);
    printf("%f\n", distance[i]); 
  }
  int z=indDMin(distance, count);
  printf("Point %d: (%f)\n", z+1, distance[z]);
  return 0;
}