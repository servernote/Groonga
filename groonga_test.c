#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <groonga.h>

double getMicroTime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (((double)tv.tv_sec)*((double)1000000)+((double)tv.tv_usec));
}

//grn_obj* getSelectParam(grn_ctx *ctx, grn_obj *obj, int index, const char *value) {
grn_obj* getSelectParam(grn_ctx *ctx, grn_obj *obj, const char *name, const char *value) {
//	grn_obj *ret = grn_expr_get_var_by_offset(ctx, obj, index);
	grn_obj *ret = grn_expr_get_var(ctx, obj, name, strlen(name));
	grn_obj_reinit(ctx, ret, GRN_DB_TEXT, 0);
	GRN_TEXT_PUTS(ctx, ret, value);
	return ret;
}

int main (int argc, char **argv) {
	grn_ctx ctx;
	grn_obj *db;
	grn_ctx_info info;
	const char *path = "/home/hogeuser/grn/db";
	double mt1,mt2;

	mt1 = getMicroTime();

	grn_init();
	grn_ctx_init(&ctx, 0);
	GRN_DB_OPEN_OR_CREATE(&ctx, path, NULL, db);
	grn_ctx_set_output_type(&ctx, GRN_CONTENT_JSON);

	mt2 = getMicroTime();
	printf("initialize library (%lfms)\n",(mt2 - mt1) / 1000);

	mt1 = getMicroTime();

/*
	grn_obj *obj = grn_ctx_get(&ctx, "select", strlen("select"));
	grn_obj *table = getSelectParam(&ctx, obj, 0, "EkiPos");
	grn_obj *filter = getSelectParam(&ctx, obj, 3, "geo_in_circle(location, \"35.681454,139.767200\", 10000)");
	grn_obj *scorer = getSelectParam(&ctx, obj, 4, "_score = geo_distance(location, \"35.681454,139.767200\")");
	grn_obj *output_columns = getSelectParam(&ctx, obj, 6, "_key,_score");
	grn_obj *sortby = getSelectParam(&ctx, obj, 5, "_score");
	grn_obj *offset = getSelectParam(&ctx, obj, 7, "0");
	grn_obj *limit = getSelectParam(&ctx, obj, 8, "100");
*/

	grn_obj *obj = grn_ctx_get(&ctx, "select", strlen("select"));
	grn_obj *table = getSelectParam(&ctx, obj, "table", "EkiPos");
	grn_obj *filter = getSelectParam(&ctx, obj, "filter", "geo_in_circle(location, \"35.681454,139.767200\", 10000)");
	grn_obj *scorer = getSelectParam(&ctx, obj, "scorer", "_score = geo_distance(location, \"35.681454,139.767200\")");
	grn_obj *output_columns = getSelectParam(&ctx, obj, "output_columns", "_key,_score");
	grn_obj *sortby = getSelectParam(&ctx, obj, "sortby", "_score");
	grn_obj *offset = getSelectParam(&ctx, obj, "offset", "0");
	grn_obj *limit = getSelectParam(&ctx, obj, "limit", "100");


	mt2 = getMicroTime();
	printf("create sql objects (%lfms)\n",(mt2 - mt1) / 1000);

	mt1 = getMicroTime();

	grn_expr_exec(&ctx, obj, 0);

	mt2 = getMicroTime();
	printf("grn_expr_exec (%lfms)\n",(mt2 - mt1) / 1000);

	mt1 = getMicroTime();

	grn_ctx_info_get(&ctx, &info);
	printf("%.*s\n", (int)GRN_TEXT_LEN(info.outbuf), GRN_TEXT_VALUE(info.outbuf));

	grn_expr_clear_vars(&ctx, obj);
	grn_obj_unlink(&ctx, obj);

	mt2 = getMicroTime();
	printf("get result print free (%lfms)\n",(mt2 - mt1) / 1000);

	mt1 = getMicroTime();

	grn_obj_close(&ctx, db);
	grn_ctx_fin(&ctx);
	grn_fin();

	mt2 = getMicroTime();
	printf("free library objects (%lfms)\n",(mt2 - mt1) / 1000);

	return 0;
}
