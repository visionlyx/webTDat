/*********************************************************************
neuronSWC.cpp
*************************************************************************/

#include "neuronSWC.h"

Single_SWC merge_most_SWC_list(SWC_list & in_swc_list)
{
	Single_SWC out_swc;  out_swc.clear();
	SWC_unit v;
	long n = 0, i, j, k;
	long nsegs = in_swc_list.seg.size();
	for (k = 0; k<nsegs; k++)
	{
		vector <SWC_unit> &row = (in_swc_list.seg.at(k).row);
		if (row.size() <= 0) continue;

		//first find the min index number, then all index will be automatically adjusted
		long min_ind = row[0].n;
		for (j = 1; j<row.size(); j++)
		{
			if (row[j].n < min_ind)  min_ind = row[j].n;
			if (min_ind<0) qDebug("Found illeagal neuron node index which is less than 0 in merge_most_SWC_list()!");
		}
		//qDebug()<<min_ind;

		// segment id & color type
		int seg_id = k;

		//now merge
		long n0 = n;
		for (j = 0; j<row.size(); j++)
		{
			v.seg_id = seg_id;
			v.nodeinseg_id = j;

			v.n = (n0 + 1) + row[j].n - min_ind;
			for (i = 1; i <= 5; i++)	v.data[i] = row[j].data[i];
			v.parent = (row[j].parent<0) ? -1 : ((n0 + 1) + row[j].parent - min_ind); //change row[j].parent<=0 to row[j].parent<0, PHC 091123.

			//qDebug()<<row[j].n<<"->"<<v.n<<" "<<row[j].parent<<"->"<<v.parent<<" "<<n;

			out_swc.row.push_back(v);
			n++;
		}
	}

	out_swc.name = "merged";
	//	for (i=0;i<out_swc.nrows();i++)
	//		qDebug()<<out_swc.row.at(i).data[2]<<" "<<out_swc.row.at(i).data[3]<<" "<<out_swc.row.at(i).data[4]<<" "<<out_swc.row.at(i).data[6];

	return out_swc;
}

vector <Single_SWC> decompose_swc(Single_SWC & in_swc)
{
	vector <Single_SWC> out_swc_segs;
	Single_SWC tempswc_seg;
	out_swc_segs.clear();
	int is_first = 1;

	tempswc_seg.append(in_swc.row.at(0));

	for (long i = 1; i<in_swc.row.size(); i++)
	{
		if (in_swc.row.at(i).data[6] == tempswc_seg.row.at(tempswc_seg.row.size() - 1).data[0])
		{
			tempswc_seg.append(in_swc.row.at(i));
		}
		else{
			if (tempswc_seg.row.size()>2)
			{
				out_swc_segs.push_back(tempswc_seg);
			}
			tempswc_seg.row.clear();
			if (in_swc.row.at(i).data[6] == -1)
			{
				tempswc_seg.append(in_swc.row.at(i));
				continue;
			}
			else{
				tempswc_seg.append(in_swc.row.at((in_swc.row.at(i).data[6]) - 1));
				tempswc_seg.append(in_swc.row.at(i));
			}
		}
	}
	if (tempswc_seg.row.size()>2)
	{
		out_swc_segs.push_back(tempswc_seg);
	}
	tempswc_seg.clear();

	return out_swc_segs;


}

void Single_SWC::save_most_SWC(const char *path) //将每个节点写出到文本文件中，即保存成SWC文件
{

	
			FILE * ffp = fopen(path, "wt");

			for (int i = 0; i<row.size(); i++)
			{
				fprintf(ffp, "%ld %ld %5.3f %5.3f %5.3f %5.3f %ld\n",
					long(row[i].data[0]), long(row[i].data[1]), row[i].data[2], row[i].data[3], row[i].data[4], row[i].data[5], long(row[i].data[6]));
			}

			fclose(ffp);
			qDebug("saved 2 OK");


	





}

void Single_SWC::load_most_SWC(const char *path)
{
	
	SWC_unit swc_unit;
	FILE *fp = fopen(path, "r");  //按照顺序读取SWC文件中的每一行，然后保存到把每一行保存在swc_unit中，然后添加在vector
	while (!feof(fp))
	{

		fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf\n",
			&(swc_unit.data[0]), &(swc_unit.data[1]), &swc_unit.data[2], &swc_unit.data[3], &swc_unit.data[4], &swc_unit.data[5], &(swc_unit.data[6]));
		append(swc_unit);
	}
	fclose(fp);
	strcpy(swcSrcFile, path);
	QFileInfo fileInfo(path);
	qDebug("Load  OK");



}