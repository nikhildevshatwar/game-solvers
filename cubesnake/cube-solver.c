#include<stdio.h>

enum direction {
	L,R,F,B,U,D
};

enum direction dirtable[][4] = {
	[L] = { F, B, U, D },
	[R] = { F, B, U, D },
	[F] = { L, R, U, D },
	[B] = { L, R, U, D },
	[U] = { L, R, F, B },
	[D] = { L, R, F, B },
};
enum rotation {
	ST, LT
};

enum rotation cubelane[27] = {
//	ST,ST,LT,ST,LT,ST,LT,ST,LT,LT,LT,LT,ST,LT,ST,LT,LT,LT,ST,LT,LT,ST,LT,LT,LT,ST,
#if 1
ST,ST,ST,LT,LT,
LT,ST,LT,LT,ST,
LT,LT,LT,ST,LT,ST,
LT,LT,LT,LT,ST,
LT,ST,LT,ST,LT,ST
#endif
#if 0
ST,ST,ST,  LT,LT,ST,  LT, LT, ST,
LT,LT,ST,  LT,LT,ST,  LT, LT, ST,
LT,LT,ST,  LT,LT,ST,  LT, LT, ST,
#endif
};

struct minmax {
	int maxi, maxj, maxk;
	int mini, minj, mink;
};

#define row5(v) { v, v, v, v, v }
#define plane5(r) { row5(r), row5(r), row5(r), row5(r), row5(r), }
#define cube5(p) { plane5(p), plane5(p), plane5(p), plane5(p), plane5(p) }

int cube[5][5][5] = cube5(-1);
int callcnt = 0, max_c = 0;

#define u_max(p) limit.max##p = p > limit.max##p ? p : limit.max##p
#define u_min(p) limit.min##p = p < limit.min##p ? p : limit.min##p
#define u_delta(p) d##p = limit.max##p - limit.min##p
#define restric(p) u_max(p); u_min(p); u_delta(p);
#define restrictIJK() restric(i); restric(j); restric(k);
#define chk_inv(i, j, k) (d##i > 2 || d##j > 2 || d##k > 2)

#define transform(i,j,k,r) \
	r==L ? k-- : r==R ? k++ : r==F ? j++ : r==B ? j-- : r==U ? i-- : r==D ? i++ : r
#define direction(d, r) \
	d==L

#define save()	do { \
				si = i; sj = j; sk = k; \
				slimit = limit; \
				sdir = dir; \
			} while(0)
#define restore()	do { \
				i = si; j = sj; k = sk; \
				limit = slimit; \
				dir = sdir; \
			} while(0)

void print_cube() {
	int i,j,k,val;
	for (i=0; i<5; i++) {
		printf("\n");
		for (j=0; j<5; j++) {
			for (k=0; k<5; k++) {
				val = cube[j][k][i];
				if (val >=0)
					printf("%3d,", val);
				else
					printf("   ,", val);
			}
			printf("     ");
		}
	}
	printf("\n");
}

int rec_cube(int c, int i, int j, int k, int dir, struct minmax limit) {
	int di,dj,dk;
	int si,sj,sk, sdir;
	struct minmax slimit;
	int ret, d;

	callcnt++;
	if (c > max_c)
		max_c = c;

	if (c == 27) {
		print_cube();
		printf("SUCCESS Total attempts %d\n", callcnt);
		return 1;
	}

	save();
	for (d = 0; d < 4; d++) {

		restore();

		if (cubelane[c] == ST) {
			if (d != 0)
				continue;
			dir = dir;
		} else {
			dir = dirtable[dir][d];
		}
		transform(i,j,k,dir);
		restrictIJK();
		if (chk_inv(i,j,k) || cube[i][j][k] != -1) {
			continue;
		}

		cube[i][j][k] = c;
		ret = rec_cube(c + 1, i,j,k, dir, limit);
		if (ret)
			return ret;
		cube[i][j][k] = -1;
	}
	return 0;
}

int main() {
  struct minmax start = {2,2,2,2,2,2};
	cube[2][2][2] = 0;
	if(!rec_cube(1, 2, 2, 2, R, start)) {
     printf("Impossible! Attempted %d ways (Max reached %d)", callcnt, max_c);
   }
};
