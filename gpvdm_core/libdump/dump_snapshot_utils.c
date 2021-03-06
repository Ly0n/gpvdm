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

/** @file dump_snapshot_utils.c
@brief Utils to handle the snapshots dir.
*/

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <exp.h>
#include "sim.h"
#include "dump.h"
#include <cal_path.h>
#include <pl.h>
#include <probe.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <inp.h>
#include <util.h>

static int unused __attribute__((unused));

void dump_remove_snapshots(struct simulation* sim)
{
	char test_dir[PATH_MAX];
	char test_file[PATH_MAX];

	DIR *d;
	struct dirent *dir;

	d = opendir(get_output_path(sim));
	if (d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			join_path(2,test_dir,get_output_path(sim),dir->d_name);
			if (isdir(test_dir)==0)
			{
				join_path(3,test_file,get_output_path(sim),dir->d_name,"snapshots.inp");
				//printf("%s\n",test_file);
				if (isfile(test_file)==0)
				{
					printf("delete: %s\n",test_dir);

					remove_dir(sim,test_dir);
				}
			}
		}
		closedir(d);
	}

}

void dump_make_snapshot_dir(struct simulation *sim,char *out_dir ,char *snapshot_name, int number)
{
	struct dat_file buf;

	char temp[200];
	char sub_dir[200];
	char snapshots_dir[PATH_MAX];
	sprintf(sub_dir,"%d",number);

	buffer_init(&buf);

	join_path(2,snapshots_dir,get_output_path(sim),snapshot_name);
	buffer_add_dir(sim,snapshots_dir);

	buffer_malloc(&buf);

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,snapshots_dir,"snapshots.inp",&buf);
	buffer_free(&buf);

	join_path(2,out_dir,snapshots_dir,sub_dir);
	buffer_add_dir(sim,out_dir);
}

void dump_make_snapshot_dir_with_name(struct simulation *sim,char *out_dir ,long double time,long  double voltage, long double fx, int number,char *snapshot_name)
{

	char temp[200];
	struct dat_file buf;

	dump_make_snapshot_dir(sim,out_dir,snapshot_name, number);

	buffer_init(&buf);


	buffer_malloc(&buf);

	sprintf(temp,"#dump_voltage\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"%Lf\n",voltage);
	buffer_add_string(&buf,temp);

	sprintf(temp,"#dump_time\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"%Lf\n",time);
	buffer_add_string(&buf,temp);

	sprintf(temp,"#dump_fx\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"%Le\n",fx);
	buffer_add_string(&buf,temp);

	sprintf(temp,"#ver\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"1.0\n");
	buffer_add_string(&buf,temp);

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,out_dir,"snapshot_info.dat",&buf);
	buffer_free(&buf);


}

void dump_make_snapshot_dir_with_info(struct simulation *sim,char *out_dir ,long double time,long  double voltage, long double fx ,int number)
{
	dump_make_snapshot_dir_with_name(sim,out_dir ,time,voltage, fx,number,"snapshots");

}

