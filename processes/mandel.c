#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#define buffer(y,x) buffer[y*nx + x]

int max_iterations = 255;

//*******************************************************************************//

int calcula_ponto(double ci, double cr) {
  int iteracoes = 0;
  double zi = 0;
  double zr = 0;
  
  while ((zr*zr + zi*zi < 4) && (iteracoes < max_iterations)) {
    double nr, ni; 

    /* Z <-- Z^2 + C */
   
    nr = zr*zr - zi*zi + cr;
    ni = 2*zr*zi + ci;
    
    zi = ni;
    zr = nr;

    iteracoes++;
  } 
  return iteracoes;  
}

//*******************************************************************************//

void calcula_madelbrot(int li, int np, int *buffer, int nx, int ny, double xmin, double xmax, double ymin, double ymax) {
  double delta_x, delta_y;
  double valor_x, valor_y;
  int x, y;

  delta_x = (xmax - xmin)/nx;
  delta_y = (ymax - ymin)/ny;

  for (y=li;  y<ny; y+=np){
    valor_y = ymin + delta_y * y;
    for (x=0; x<nx; x++) {
      valor_x = xmin + delta_x * x;
      buffer(y,x) = calcula_ponto(valor_x, valor_y);
    }
  }
}

//*******************************************************************************//
void gera_arquvio_ppm(char *nome_arquivo, int *buffer, int nx, int ny, int max) {
  int i;

  FILE *file = fopen(nome_arquivo,"w");

  fprintf(file,"P2\n");
  fprintf(file,"%d %d\n",nx,ny);

  fprintf(file,"%d",max);

  for (i=0; i<nx*ny; i++) {
    if (!(i%nx)){ 
	fprintf(file,"\n");
    }	
    fprintf(file,"%d ",buffer[i]);
  }
  fclose(file);
}

//*******************************************************************************//
int main( int argc,  char **argv ){
	int nx = 5000;
	int ny = 5000;
		
	double xmin = -1.5;
	double ymin = -1.5;
	double xmax = 1.5;
	double ymax = 1.5;

	if (argc != 2){
		printf("errouuuuu");
		exit(0);
	}

	int key = 5;
	int shmid = shmget(key, nx * ny * sizeof(int), IPC_CREAT | 0600);
	int *buffer = shmat(shmid, 0, 0);
	
	int np = atoi(argv[1]);
	int i, id, li;
	li=0;
	for(i=1;i<np;i++) {
		id = fork();
		if (id == 0){
			li=i;
			break;
		}
	}	
	
	
	calcula_madelbrot(li, np, buffer, nx, ny, xmin, xmax, ymin, ymax);

	if (id == 0){
		for(i = 1; i<np; i++){
			wait(NULL);
		}
		gera_arquvio_ppm("saida.ppm", buffer, nx, ny, 255);
		shmdt(buffer);
		shmctl(shmid, IPC_RMID, 0);
	} else {
		shmdt(buffer);	
	}
	
	
}
//*******************************************************************************//
	
	




