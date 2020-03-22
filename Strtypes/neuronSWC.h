
#ifndef NEURONSWC_H
#define NEURONSWC_H

#include <QtGui>
#include <vector>
#include <string>
#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
using namespace std;


struct SWC_unit     //����SWC�ڵ��࣬��һ�������ʾswc�ĸ����ڵ�
{
	union {
		double data[10];
		struct {
			double n, type, x, y, z, r, parent,
				nchild,
				seg_id, nodeinseg_id;
		};
	};
	SWC_unit() { for (long i = 0; i<long(sizeof(data) / sizeof(double)); i++) data[i] = 0; r = 0.5; } //���캯��
	void set(double x1, double y1, double z1, double r1, double p1, double t1) { x = x1; y = y1; z = z1; r = r1; parent = p1; type = t1; } //set����
	void set(double x1, double y1, double z1, double r1, double p1) { x = x1; y = y1; z = z1; r = r1; parent = p1; }
	void set(double x1, double y1, double z1, double r1) { x = x1; y = y1; z = z1; r = r1; }
	void set(double x1, double y1, double z1) { x = x1; y = y1; z = z1; }
};

struct Single_SWC  //swc�ļ��࣬
{
	vector<SWC_unit> row;  //������swc�ڵ���װ��vector��
	int swc_type;     //swc���ͣ���ʾѪ�ܣ����嶨λ��ȵ�
	bool is_checked;   //��GUI�������Ƿ�ѡ��
	bool is_modified;   //�Ƿ��޸�
	string name;      //����
	string comment;
	string file;     //�ļ�·��
	int parent_image_id;   //���ĸ�ͼ��ָ�����Ľ�������ָ��ͼ���ID��
	bool b_jointed;
	char swcSrcFile[1024]; //the full file path
	char swcName[128];
	vtkSmartPointer<vtkActor> actor;  //���ڿ��ӻ�ʱ���actor�Ƿ��Ѿ�



	Single_SWC(string new_name = "unset", bool is_linegraph = false){

		swc_type = 0;
		name = new_name; is_checked = is_linegraph;   b_jointed = false;
		swcSrcFile[0] = '\0'; swcName[0] = '*'; swcName[1] = '\0'; is_modified = true;
		parent_image_id = -1;
		actor = NULL;
	}



	//signals:
	//void Single_SWC();
	int getparentID() { return parent_image_id; }  //get��set����
	void setparentID(int i) { parent_image_id = i; }
	int getswctype(){ return swc_type; }
	void setswctype(int i){ swc_type = i; }
	long nrows() { return row.size(); }  //�ܹ��ж���SWC�ڵ�
	void append(SWC_unit & new_node) { row.push_back(new_node); } //���һ���ڵ�
	void clear() { row.clear(); } //��vector���
	bool get_ischecked() { return is_checked; }
	bool get_ismodified() { return is_modified; }
	string getname(){ return name; }
	const char * getswcFileNamePath() const { return swcSrcFile; } //��ȡ����·��
	const char * getswcName() const { return swcName; } //��ȡswc�ļ�����
	void setmost_SWC(const Single_SWC* temp)  //��һ��most_SWC��Ķ��󿽱�����һ����
	{

		is_checked = temp->is_checked;
		is_modified = temp->is_modified;
		name = temp->name;
		comment = temp->comment;
		file = temp->file;
		b_jointed = temp->b_jointed;
		swc_type = temp->swc_type;
		parent_image_id = temp->parent_image_id;
		strcpy(swcSrcFile, temp->swcSrcFile);
		strcpy(swcName + 1, temp->swcName);

		row = (*temp).row;
		actor = temp->actor;
		//row.resize(temp->row.size());
		//memcpy(&row[0],&temp->row[0],temp->row.size()*sizeof(most_SWC_unit));
	}



	void save_most_SWC(const char *path); //����swc�ļ�
	void load_most_SWC(const char *path);//��ȡswc�ļ�




};
//�ܶ��swc����ɵ�vector�������������Ҫ�����ڷ�Ѫ�ܵ�ʱ��
struct SWC_list
{
	vector <Single_SWC> seg; //since each seg could be a complete neuron or multiple paths, thus I call it "seg", but not "path"
	string name;
	string comment;
	string file;
	bool b_traced;

	SWC_list() { b_traced = true; }

	long nsegs() { return seg.size(); } //���ض��ٸ�swc�ļ�����
	long nrows() { long n = 0; for (long i = 0; i<(long)seg.size(); i++) n += seg.at(i).nrows(); return n; }
	bool isJointed() { return nsegs() == 1 && seg.at(0).b_jointed; }
	void append(Single_SWC & new_seg) { seg.push_back(new_seg); }
	void append(vector <Single_SWC> & new_segs) { for (int k = 0; k<(int)new_segs.size(); k++) seg.push_back(new_segs.at(k)); }
	void clear() { seg.clear(); }
};

Single_SWC merge_most_SWC_list(SWC_list & in_swc_list);  //��most_SWC_list�ļ��ϲ���һ��swc�ļ�
vector <Single_SWC> decompose_swc(Single_SWC & in_swc);   //��һ��swc�ļ���ֳɶ��most_SWC_list�ļ�

Q_DECLARE_METATYPE(Single_SWC*)

#endif