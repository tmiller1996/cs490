#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct team_pair team_pair;

#define ERROR(...){printf("Error: "); \
	printf(__VA_ARGS__); \
	putchar('\n'); \
	exit(EXIT_FAILURE);}

#define MIN(a, b)\
	({__typeof__(a) _a = (a); \
	  __typeof__(b) _b = (b);\
	  _a < _b ? _a : _b;\
	})

#define MAX(a, b)\
	({__typeof__(a) _a = (a); \
	  __typeof__(b) _b = (b);\
	  _a > _b ? _a : _b;\
	})

struct team_pair{
	int a, b;
};

// initialized from argument
int size;

int main(int argc, char **argv){
	// parse arguments
	if(argc != 2)
		ERROR("incorrect sizeumber of args.  Expected %d, got %d", 1, (argc-1));
	size = atoi(argv[1]);

	// [MIN team][MAX team]
	bool table_team_pairs[size*2][size*2];
	for(int i = 0; i < size*2; i++){
		for(int j = 0; j < size*2; j++){
			table_team_pairs[i][j] = false;
		}
	}
	// [team][time]
	bool table_team_v_time[size*2][size];
	// [team][event]
	bool table_team_v_event[size*2][size];
	for(int i = 0; i < size*2; i++){
		for(int j = 0; j < size; j++){
			table_team_v_time[i][j] = false;
			table_team_v_event[i][j] = false;
		}
	}

	// [time][event]
	team_pair schedule[size][size];
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			schedule[i][j] = (team_pair){-1, -1};
		}
	}

	// iterate times
	for(int time = 0; time < size; time++){
		// iterate events
		int event;
		for(event = 0; event < size; event++){
			// find team that is free this time for this event
			int team1;
			for(team1 = 0; team1 < size*2; team1++){
				// end loop early if team is elligible
				if(!(table_team_v_event[team1][event]) && 
					!(table_team_v_time[team1][time])){
					break;
				}
			}
			table_team_v_time[team1][time] = true;
			table_team_v_event[team1][event] = true;
			// find second team that is free this time for this event
			// team2 cannot have played team1
			int team2;
			for(team2 = 0; team2 < size*2; team2++){
				if(!(table_team_v_event[team2][event]) &&
					!(table_team_v_time[team2][time])  &&
					!table_team_pairs[
						MIN(team1, team2)][
						MAX(team1, team2)]){
					break;
				}
			}
			// update tables
			table_team_v_time[team2][time] = true;
			table_team_v_event[team2][event] = true;
			table_team_pairs[MIN(team1, team2)][MAX(team1, team2)] = true;
			// printf("%d %d %d %d\n", time, event, team1, team2);
			schedule[time][event] = (team_pair){team1, team2};
		}
	}

	// print schedule
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			printf("{%d, %d} ",
				schedule[i][j].a,
				schedule[i][j].b);
		}
		putchar('\n');
	}
	printf("Succeeded for N = %d\n", size);
}