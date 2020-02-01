
#include "sexy_db_mysql.h"
#include <stdio.h>
class my_db : public sexy_db_mysql::db_mysql{
public:
	virtual void work_on_error(int errorid,const char* msg){
		int i=0;
	}
	virtual void work_on_result(const char* sql,void* arg,int errorid,long* effect_rows_num,long* insert_new_id,sexy_db_mysql::db_res* res){
		sexy_db_mysql::mem_db_res r=sexy_db_mysql::mem_db_res::create(res);
		unsigned int fn=r.num_fields();
		long rn=r.num_rows();
		for(long i=0;i<rn;i++){
			printf("row %d\n",i);
			sexy_db_mysql::mem_db_row* row=r.fetch_next_row();
			for(unsigned int j=0;j<fn;j++){
				int x;
				const char* d=row->fetch_column(j,&x);
				if(d==0){d="null";}
				printf("<%s=%d,%s>",r.fetch_column_name(j),x,d);
			}
			printf("\n");
		}
		sexy_db_mysql::mem_db_res::destroy(r);
	}
};
int main(int argc,char* argv[]){
	my_db md;
	md.init("127.0.0.1","3306","octgame","root","123456");
	md.query("select * from player",0,1,1,1);
	md.dest();
	return 0;
}
