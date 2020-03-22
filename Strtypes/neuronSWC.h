
#ifndef NEURONSWC_H
#define NEURONSWC_H

#include <QtGui>
#include <vector>
#include <string>
#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
using namespace std;


struct SWC_unit     //单个SWC节点类，用一个数组表示swc的各个节点
{
	union {
		double data[10];
		struct {
			double n, type, x, y, z, r, parent,
				nchild,
				seg_id, nodeinseg_id;
		};
	};
	SWC_unit() { for (long i = 0; i<long(sizeof(data) / sizeof(double)); i++) data[i] = 0; r = 0.5; } //构造函数
	void set(double x1, double y1, double z1, double r1, double p1, double t1) { x = x1; y = y1; z = z1; r = r1; parent = p1; type = t1; } //set函数
	void set(double x1, double y1, double z1, double r1, double p1) { x = x1; y = y1; z = z1; r = r1; parent = p1; }
	void set(double x1, double y1, double z1, double r1) { x = x1; y = y1; z = z1; r = r1; }
	void set(double x1, double y1, double z1) { x = x1; y = y1; z = z1; }
};

struct Single_SWC  //swc文件类，
{
	vector<SWC_unit> row;  //将单个swc节点类装载vector中
	int swc_type;     //swc类型，表示血管，胞体定位点等等
	bool is_checked;   //在GUI界面中是否被选中
	bool is_modified;   //是否被修改
	string name;      //名字
	string comment;
	string file;     //文件路径
	int parent_image_id;   //由哪个图像分割出来的结果，即分割的图像的ID号
	bool b_jointed;
	char swcSrcFile[1024]; //the full file path
	char swcName[128];
	vtkSmartPointer<vtkActor> actor;  //用于可视化时候的actor是否已经



	Single_SWC(string new_name = "unset", bool is_linegraph = false){

		swc_type = 0;
		name = new_name; is_checked = is_linegraph;   b_jointed = false;
		swcSrcFile[0] = '\0'; swcName[0] = '*'; swcName[1] = '\0'; is_modified = true;
		parent_image_id = -1;
		actor = NULL;
	}



	//signals:
	//void Single_SWC();
	int getparentID() { return parent_image_id; }  //get和set方法
	void setparentID(int i) { parent_image_id = i; }
	int getswctype(){ return swc_type; }
	void setswctype(int i){ swc_type = i; }
	long nrows() { return row.size(); }  //总共有多少SWC节点
	void append(SWC_unit & new_node) { row.push_back(new_node); } //添加一个节点
	void clear() { row.clear(); } //将vector清除
	bool get_ischecked() { return is_checked; }
	bool get_ismodified() { return is_modified; }
	string getname(){ return name; }
	const char * getswcFileNamePath() const { return swcSrcFile; } //获取完整路径
	const char * getswcName() const { return swcName; } //获取swc文件名字
	void setmost_SWC(const Single_SWC* temp)  //将一个most_SWC类的对象拷贝到另一个中
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



	void save_most_SWC(const char *path); //保存swc文件
	void load_most_SWC(const char *path);//读取swc文件




};
//很多歌swc类组成的vector，即多个树，主要有用在分血管的时候
struct SWC_list
{
	vector <Single_SWC> seg; //since each seg could be a complete neuron or multiple paths, thus I call it "seg", but not "path"
	string name;
	string comment;
	string file;
	bool b_traced;

	SWC_list() { b_traced = true; }

	long nsegs() { return seg.size(); } //返回多少个swc文件数量
	long nrows() { long n = 0; for (long i = 0; i<(long)seg.size(); i++) n += seg.at(i).nrows(); return n; }
	bool isJointed() { return nsegs() == 1 && seg.at(0).b_jointed; }
	void append(Single_SWC & new_seg) { seg.push_back(new_seg); }
	void append(vector <Single_SWC> & new_segs) { for (int k = 0; k<(int)new_segs.size(); k++) seg.push_back(new_segs.at(k)); }
	void clear() { seg.clear(); }
};

Single_SWC merge_most_SWC_list(SWC_list & in_swc_list);  //将most_SWC_list文件合并成一个swc文件
vector <Single_SWC> decompose_swc(Single_SWC & in_swc);   //将一个swc文件拆分成多个most_SWC_list文件

Q_DECLARE_METATYPE(Single_SWC*)

#endif