//
// General-purpose Photovoltaic Device Model gpvdm.com - a drift diffusion
// base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// The model can simulate OLEDs, Perovskite cells, and OFETs.
// 
// Copyright (C) 2008-2020 Roderick C. I. MacKenzie
// 
// https://www.gpvdm.com
// r.c.i.mackenzie at googlemail.com
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the GPVDM nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL Roderick C. I. MacKenzie BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 

/** @file ligh_dump_verbose_1d.c
	@brief Dumps 1D optical fields from light model.
*/


#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "gpvdm_const.h"
#include "dump_ctrl.h"
#include "light.h"
#include "dat_file.h"
#include <cal_path.h>
#include <lang.h>

void light_dump_verbose_1d(struct simulation *sim,struct light *li, int l,char *ext)
{
	return;
	char line[1024];
	char temp[1024];
	char name_photons[200];
	char name_light_1d_Ep[200];
	char name_light_1d_En[200];
	char name_pointing[200];
	char name_E_tot[200];
	char name_r[200];
	char name_t[200];
	char name[200];

	int x=0;
	int z=0;
	int y=0;

	struct epitaxy *epi=li->epi;
	long double device_start=epi->device_start;

	//int max=0;
	struct dat_file data_photons;
	struct dat_file data_light_1d_Ep;
	struct dat_file data_light_1d_En;
	struct dat_file data_pointing;
	struct dat_file data_E_tot;

	struct dat_file data_r;
	struct dat_file data_t;

	struct dat_file buf;
	struct dim_light *dim=&li->dim;

	buffer_init(&data_light_1d_Ep);
	buffer_init(&data_light_1d_En);
	buffer_init(&data_pointing);
	buffer_init(&data_E_tot);
	buffer_init(&buf);

	buffer_init(&data_r);
	buffer_init(&data_t);
	buffer_init(&data_photons);

	buffer_malloc(&data_photons);
	buffer_malloc(&data_light_1d_Ep);
	buffer_malloc(&data_light_1d_En);
	buffer_malloc(&data_pointing);
	buffer_malloc(&data_E_tot);
	buffer_malloc(&data_r);
	buffer_malloc(&data_t);

	sprintf(name_photons,"light_1d_%.0Lf_photons%s.dat",dim->l[l]*1e9,ext);

	sprintf(name_light_1d_Ep,"light_1d_%.0Lf_Ep%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_light_1d_En,"light_1d_%.0Lf_En%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_pointing,"light_1d_%.0Lf_pointing%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_E_tot,"light_1d_%.0Lf_E_tot%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_r,"light_1d_%.0Lf_r%s.dat",dim->l[l]*1e9,ext);
	sprintf(name_t,"light_1d_%.0Lf_t%s.dat",dim->l[l]*1e9,ext);

	//
	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,li->photons[z][x][y][l]);
		buffer_add_string(&data_photons,line);

		sprintf(line,"%Le %Le %Le %Le\n",dim->y[y]-device_start,gpow(gpow(li->Ep[z][x][y][l],2.0)+gpow(li->Epz[z][x][y][l],2.0),0.5),li->Ep[z][x][y][l],li->Epz[z][x][y][l]);
		buffer_add_string(&data_light_1d_Ep,line);

		sprintf(line,"%Le %Le %Le %Le\n",dim->y[y]-device_start,gpow(gpow(li->En[z][x][y][l],2.0)+gpow(li->Enz[z][x][y][l],2.0),0.5),li->En[z][x][y][l],li->Enz[z][x][y][l]);
		buffer_add_string(&data_light_1d_En,line);

		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,li->pointing_vector[z][x][y][l]);
		buffer_add_string(&data_pointing,line);

		sprintf(line,"%Le %Le %Le\n",dim->y[y]-device_start,li->E_tot_r[z][x][y][l],li->E_tot_i[z][x][y][l]);
		buffer_add_string(&data_E_tot,line);

		sprintf(line,"%Le %Le %Le %Le\n",dim->y[y]-device_start,gcabs(li->r[z][x][y][l]),gcreal(li->r[z][x][y][l]),gcimag(li->r[z][x][y][l]));
		buffer_add_string(&data_r,line);

		sprintf(line,"%Le %Le %Le %Le\n",dim->y[y]-device_start,gcabs(li->t[z][x][y][l]),gcreal(li->t[z][x][y][l]),gcimag(li->t[z][x][y][l]));
		buffer_add_string(&data_t,line);

	}

	buffer_dump_path(sim,li->dump_dir,name_photons,&data_photons);

	buffer_dump_path(sim,li->dump_dir,name_light_1d_Ep,&data_light_1d_Ep);
	buffer_dump_path(sim,li->dump_dir,name_light_1d_En,&data_light_1d_En);
	buffer_dump_path(sim,li->dump_dir,name_pointing,&data_pointing);
	buffer_dump_path(sim,li->dump_dir,name_E_tot,&data_E_tot);
	buffer_dump_path(sim,li->dump_dir,name_r,&data_r);
	buffer_dump_path(sim,li->dump_dir,name_t,&data_t);



	buffer_free(&data_photons);

	buffer_free(&data_light_1d_Ep);
	buffer_free(&data_light_1d_En);
	buffer_free(&data_pointing);
	buffer_free(&data_E_tot);
	buffer_free(&data_r);
	buffer_free(&data_t);

	buffer_malloc(&buf);
	dim_light_info_to_buf(&buf,dim);
	strcpy(buf.title,"Real refractive index (n) vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,_("Real refractive index"));
	strcpy(buf.data_units,"au");
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y],li->n[0][0][y][l]);
		buffer_add_string(&buf,line);
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(name,"light_1d_%.0Lf_n%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,name,&buf);
	buffer_free(&buf);




	buffer_malloc(&buf);
	dim_light_info_to_buf(&buf,dim);
	strcpy(buf.title,"Absorption vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.data_label,_("Absorption"));
	strcpy(buf.data_units,"m^{-1}");
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y],li->alpha[0][0][y][l]);
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(name,"light_1d_%.0Lf_alpha%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"|Electric field| vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("|Electric field|"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"V/m");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,gpow(gpow(li->Ep[z][x][y][l]+li->En[z][x][y][l],2.0)+gpow(li->Enz[z][x][y][l]+li->Epz[z][x][y][l],2.0),0.5));
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"light_1d_%.0Lf_E%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,temp,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Transmittance vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("Transmittance"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"au");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,gcabs(li->t[z][x][y][l]));
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"light_1d_%.0Lf_t%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,temp,&buf);
	buffer_free(&buf);



	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Reflectance vs position");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.data_label,_("Reflectance"));
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"au");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=dim->ylen;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);

	for (y=0;y<dim->ylen;y++)
	{
		sprintf(line,"%Le %Le\n",dim->y[y]-device_start,gcabs(li->r[z][x][y][l]));
		buffer_add_string(&buf,line);
	}


	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"light_1d_%.0Lf_r%s.dat",dim->l[l]*1e9,ext);
	buffer_dump_path(sim,li->dump_dir,temp,&buf);
	buffer_free(&buf);

}
