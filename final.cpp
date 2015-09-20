#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <sstream>
using namespace std;
class Register {
private:
	int regist[9];
public:
	Register() {
		this->regist[0]=0;
		this->regist[1]=5;
		this->regist[2]=8;
		this->regist[3]=6;
		this->regist[4]=7;
		this->regist[5]=5;
		this->regist[6]=1;
		this->regist[7]=2;
		this->regist[8]=4;
	}
	//值的設定
	void set(int i, int n) {
		if (i!=0)
			this->regist[i]=n;
	}
	//值的取得
	int get(int i) {
		return this->regist[i];
	}
	//顯示目前register內容的資訊
	string show() {
		string show("Register:\n");
		//cout << "Register:" << endl;
		for (int i=0;i<3;i++) {
			for (int j=0;j<3;j++) {
				int a=3*i+j;
				int b=regist[3*i+j];
				show=show+string("$")+int2str(a)+string(": ")+int2str(b);
				//cout << "$" << (3*i+j) << ": "<< this->regist[3*i+j];
				if (j!=2) {
					show=show+string("\t");
					//cout << "\t";
				}
			}
			show=show+string("\n");
			//cout << endl;
		}
		return show;
	}
	string int2str(int &i) {
		string s;
		stringstream ss(s);
		ss << i;
		return ss.str();
	}
};
class Memory {
private:
	int mem[5];
public:
	Memory() {
		this->mem[0]=9;
		this->mem[1]=6;
		this->mem[2]=4;
		this->mem[3]=2;
		this->mem[4]=5;
	}
	//存到記憶體
	void set(int i, int n) {
		if (i%4!=0)
			throw "Memory set number error!";
		this->mem[i/4]=n;
	}
	//從memery裡取得資料
	int get(int i) {
		if (i%4!=0)
				throw "Memory get number error!";
		return this->mem[i/4];
	}
	//顯示memery裡的資料
	string show() {
		string show("Data memory:\n");
		//cout << "Data memory:" << endl;
		for (int i=0;i<5;i++) {
			int a=4*i;
			int b=mem[i];
			show=show+int2str(a)+string(":\t")+int2str(b)+string("\n");
			//cout << setw(2) << setfill('0') << 4*i << ":\t"<< this->mem[i] << endl;
		}
		return show;
	}
	string int2str(int &i) {
		string s;
		stringstream ss(s);
		ss << i;
		string out=ss.str();
		if (i<10)
			out=string("0")+out;
		return out;
	}
};
class Instruction {
private:
	int bit[32];
	int getNumber(int a, int b) {
		int sum=0;
		for (int i=a;i<b;i++) {
			sum=sum*2+this->bit[i];
		}
		return sum;
	}
public:
	Instruction() {
		for (int i=0; i<32;i++) {
			this->bit[i]=0;
		}
		this->bit[26]=0;
	}
	Instruction(char *s) {
		for (int i=0; i<32;i++) {
			this->bit[i]=((int)*(s+i))-48;
		}
	}
	// lw is 0
	// sw is 1
	// add is 2
	// sub is 3
	// and is 4
	// or is 5
	// beq is 6
	// slt is 7
	int getInstruction() {
		int n=getNumber(0, 6);
		if (n==0) {
			n=getNumber(26, 32);
			if (n==32)
				return 2;
			else if (n==34)
				return 3;
			else if (n==36)
				return 4;
			else if (n==37)
				return 5;
			else if (n==42)
				return 7;
			else if (n==0)
				return 8;
			//cout << "function: << n << endl;
			n=-1;
		}
		else if (n==35) 
			return 0;
		else if (n==43)
			return 1;
		else if (n==4)
			return 6;
		//cout << "format: " << n << endl;
		return -1;
	}
	int be_correct() {
		int n=getInstruction();
		if (n==-1)
			return 0;
		else
			return 1;
	}
	string get_bit_string() {
		string s;
		string one=string("1");
		string zero=string("0");
		for (int i=0;i<32;i++) {
			if (bit[i])
				s=s+one;
			else
				s=s+zero;
		}
		return s;
	}
	string toString() {
		int n=getInstruction();
		if (n==-1)
			return string( "can't judge this instruction.");
		else {
			string inst;
			if (n==0)
				inst=string("lw");
			else if (n==1)
				inst=string("sw");
			else if (n==2)
				inst=string("add");
			else if (n==3)
				inst=string("sub");
			else if (n==4)
				inst=string("and");
			else if (n==5)
				inst=string("or");
			else if (n==6)
				inst=string("beq");
			else if (n==7)
				inst=string("slt");
			else
				inst=string("sll");
			int rs=get_rs();
			int rt=get_rt();
			int rd=get_rd();
			int imm=get_imm();
			string t("\t$");
			string d(", $");
			string e("\n");
			string srs=int2str(rs);
			string srt=int2str(rt);
			string srd=int2str(rd);
			string simm=int2str(imm);
			if (n==0 || n==1) { 
				if (imm<10)
					return inst+t+srt+d+string("0")+simm+string("($")+srs+string(")\n");
				else
					return inst+t+srt+d+simm+string("($")+srs+string(")\n");
				//cout << inst << "\t$" << rt << ", 0x" << setw(2) << setfill('0') <<  imm << "($" << rs << ")" << endl;
			}
			else if (n==6) {
				if (imm<10)
					return inst+t+srt+d+srs+string(", 0x0")+simm+e;
				else
					return inst+t+srt+d+srs+string(", 0x")+simm+e;
				//cout << inst << "\t$" << rt << ", $" << rs << ", 0x" << setw(2) << setfill('0') << imm << endl;
			}
			else  {
				return inst+t+srd+d+srs+d+srt+e;
				//cout << inst << "\t$" << rd << ", $" << rs << ", $" << rt << endl;
			}
		}
	}
	string int2str(int &i) {
		string s;
		stringstream ss(s);
		ss << i;
		return ss.str();
	}
	int get_rs() {
		return getNumber(6, 11);
	}
	int get_rt() {
		return getNumber(11, 16);
	}
	int get_rd() {
		return getNumber(16, 21);
	}
	int get_imm() {
		return getNumber(16, 32);
	}
};
class Reg1 {
public:
	Instruction *inst;
	Reg1() {
		this->inst=new Instruction();
	}
	void copy(Reg1 *reg) {
		this->inst=reg->inst;
	}
};
class Reg2 {
public:
	int EX[4];
	int MEM[3];
	int WB[2];
	int data1;
	int data2;
	int rs;
	int rt;
	int rd;
	int imm;
	Reg2() {
		this->EX[0]=0; this->EX[1]=0; this->EX[2]=0; this->EX[3]=0;
		this->MEM[0]=0; this->MEM[1]=0; this->MEM[2]=0;
		this->WB[0]=0; this->WB[1]=0;
		this->data1=0; this->data2=0; this->rs=0; this->rt=0; this->rd=0; this->imm=0;
	}
	void copy(Reg2 *reg) {
		this->EX[0]=reg->EX[0];
		this->EX[1]=reg->EX[1];
		this->EX[2]=reg->EX[2];
		this->EX[3]=reg->EX[3];
		this->MEM[0]=reg->MEM[0];
		this->MEM[1]=reg->MEM[1];
		this->MEM[2]=reg->MEM[2];
		this->WB[0]=reg->WB[0];
		this->WB[1]=reg->WB[1];
		this->data1=reg->data1;
		this->data2=reg->data2;
		this->rs=reg->rs;
		this->rt=reg->rt;
		this->rd=reg->rd;
		this->imm=reg->imm;
	}
	void control_zero() {
		this->EX[0]=0;
		this->EX[1]=0;
		this->EX[2]=0;
		this->EX[3]=0;
		this->MEM[0]=0;
		this->MEM[1]=0;
		this->MEM[2]=0;
		this->WB[0]=0;
		this->WB[1]=0;
	}
	string get_control() {
		string s;
		string one=string("1");
		string zero=string("0");
		for (int i=0;i<4;i++) {
			if (this->EX[i])
				s=s+one;
			else
				s=s+zero;
		}
		for (int i=0;i<3;i++) {
			if (this->MEM[i])
				s=s+one;
			else
				s=s+zero;
		}
		for (int i=0;i<2;i++) {
			if (this->WB[i])
				s=s+one;
			else
				s=s+zero;
		}
		return s;
	}
};
class Reg3 {
public:
	int MEM[3];
	int WB[2];
	int result;
	int wdata;
	int rd;
	Reg3() {
		this->MEM[0]=0; this->MEM[1]=0; this->MEM[2]=0;
		this->WB[0]=0;this->WB[1]=0;
		this->result=0; this->wdata=0; this->rd=0;
	}
	void pass(Reg2 *reg) {
		this->MEM[0]=reg->MEM[0];
		this->MEM[1]=reg->MEM[1];
		this->MEM[2]=reg->MEM[2];
		this->WB[0]=reg->WB[0];
		this->WB[1]=reg->WB[1];
	}
	void copy(Reg3 *reg) {
		this->MEM[0]=reg->MEM[0];
		this->MEM[1]=reg->MEM[1];
		this->MEM[2]=reg->MEM[2];
		this->WB[0]=reg->WB[0];
		this->WB[1]=reg->WB[1];
		this->result=reg->result;
		this->wdata=reg->wdata;
		this->rd=reg->rd;
	}
	string get_control() {
		string s;
		string one=string("1");
		string zero=string("0");
		for (int i=0;i<3;i++) {
			if (this->MEM[i])
				s=s+one;
			else
				s=s+zero;
		}
		for (int i=0;i<2;i++) {
			if (this->WB[i])
				s=s+one;
			else
				s=s+zero;
		}
		return s;
	}
};
class Reg4 {
public:
	int WB[2];
	int data1;
	int data2;
	int rd;
	Reg4() {
		this->WB[0]=0; this->WB[1]=0;
		this->data1=0; this->data2=0; this->rd=0;
	}
	void pass(Reg3 *reg) {
		this->WB[0]=reg->WB[0];
		this->WB[1]=reg->WB[1];
	}
	void copy(Reg4 *reg) {
		this->WB[0]=reg->WB[0];
		this->WB[1]=reg->WB[1];
		this->data1=reg->data1;
		this->data2=reg->data2;
		this->rd=reg->rd;
	}
	string get_control() {
		string s;
		string one=string("1");
		string zero=string("0");
		for (int i=0;i<2;i++) {
			if (this->WB[i])
				s=s+one;
			else
				s=s+zero;
		}
		return s;
	}
};

class Pipeline {
private:
	Instruction inst[5];
	Instruction empty;
	int cc;
	int size;
	Register reg;
	Memory mem;
	Reg1 reg1, ist1;
	Reg2 reg2, ist2;
	Reg3 reg3, ist3;
	Reg4 reg4, ist4;
	int pc;
	int spical;
	void produce_control(Instruction *inst, Reg2 *reg) {
		int n=inst->getInstruction();
		int con[9][9]={
				{0, 0, 0, 1, 0, 1, 0, 1, 1}, //lw
				{0, 0, 0, 1, 0, 0, 1, 0, 0}, //sw
				{1, 1, 0, 0, 0, 0, 0, 1, 0}, //add
				{1, 1, 0, 0, 0, 0, 0, 1, 0}, //sub
				{1, 1, 0, 0, 0, 0, 0, 1, 0}, //and
				{1, 1, 0, 0, 0, 0, 0, 1, 0}, //or
				{0, 0, 1, 0, 1, 0, 0, 0, 0}, //beq
				{1, 1, 0, 0, 0, 0, 0, 1, 0}, //slt
				{1, 1, 0, 0, 0, 0, 0, 1, 0}, //sll
		};
		reg->EX[0]=con[n][0];
		reg->EX[1]=con[n][1];
		reg->EX[2]=con[n][2];
		reg->EX[3]=con[n][3];
		reg->MEM[0]=con[n][4];
		reg->MEM[1]=con[n][5];
		reg->MEM[2]=con[n][6];
		reg->WB[0]=con[n][7];
		reg->WB[1]=con[n][8];
	}
	void IF() {
		//cout << endl <<"-----     IF state     -----" << endl;
		
		if (this->pc<this->size) {
			this->reg1.inst=&inst[pc];
		}
		else {
			this->reg1.inst=&(this->empty);
		}
		this->pc++;
		//cout << "PC: " << pc << endl;
		//cout << "     ";
		//this->reg1.inst->toString();
	}
	void ID() {
		//cout << endl << "-----     ID state     -----" << endl;
		//ist1.inst->toString();
		int rs=this->ist1.inst->get_rs();
		int rt=this->ist1.inst->get_rt();
		int rd=this->ist1.inst->get_rd();
		int imm=this->ist1.inst->get_imm();
		produce_control(ist1.inst, &reg2);
		//judge is lw hazard
		//cout << "ID/EX.MemRead: " << ist2.MEM[1] << " ID/EX.RegRt: " << ist2.rt << " rs: " << rs << " rt: " << rt << endl;
		if (this->ist2.MEM[1]  && (this->ist2.rt==rs || this->ist2.rt==rt)) {
			//out("happen lw hazard");
			//cout << "EX[1]: " << ist2.EX[1] << " EX[2]: " << ist2.EX[2] << endl;
			//cout << "rerun this :" ;
			//this->reg1.inst->toString();
			this->reg1.inst=this->ist1.inst;
			this->pc--;
			//cout << "lw	PC: " << pc << endl;
			this->reg2.control_zero();
		}
		this->reg2.data1=this->reg.get(rs);
		this->reg2.data2=this->reg.get(rt);
		this->reg2.rs=rs;
		this->reg2.rt=rt;
		this->reg2.rd=rd;
		this->reg2.imm=imm;

		//judge is beq hazard
		//cout << "reg2.MEM[0]: " << reg2.MEM[0] << " reg3.MEM[0]: " << reg3.MEM[0] << endl;
		if (reg2.MEM[0]) {
			if (ist2.WB[0] && ist2.rd!=0 && (ist2.rd==rs || ist2.rd==rt)) {
				//cout << "delete ";
				//this->reg1.inst->toString();
				this->reg1.inst=&(this->empty);
				pc=pc-2;
				//cout << "happen R format and beq" << endl;
			}
			else if (reg2.data1==reg2.data2) {
				//cout << "delete ";
				//this->reg1.inst->toString();
				this->reg1.inst=&(this->empty);
				this->pc=this->pc+imm-1;
				//cout << "jump " << imm << " instrance" << endl;
				//cout << "pc: " << pc << endl;
				//out("happen beq hazard");
			}
		}
		//cout << "DI pass: EX[1]: " << reg2.EX[1] << " EX[2]: " << reg2.EX[2] << endl;
	}
	void EX() {
		//cout << endl << "-----     EX state     -----" << endl;
		//cout << "EX[1]: " << ist2.EX[1] << " EX[2]: " << ist2.EX[2] << endl;
		this->reg3.pass(&(this->ist2));
		int data1=this->ist2.data1;
		int data2=this->ist2.data2;

		// EX hazard
		//cout << "     reg write: " << ist3.WB[0] << 	" EX/MEM.Reg: " << ist3.rd << " EX/MEM.RegRd: " << ist3.rd << " ID/EX.RegRs: " << ist2.rs << endl;
		if (this->ist3.WB[0] && this->ist3.rd!=0 && this->ist3.rd==this->ist2.rs) {
			//out("rs happen forwarding in EX");
			data1=this->ist3.result;
		}
		if (this->ist3.WB[0] && this->ist3.rd!=0 && this->ist3.rd==this->ist2.rt) {
			//out("rt happen forwarding in EX");
			data2=this->ist3.result;
		}

		// MEM hazard
		if (this->ist4.WB[0] && this->ist4.rd!=0 && this->ist3.rd!=this->ist2.rs && this->ist4.rd==this->ist2.rs) {
			//out("rs happen forwarding in MEM");
			if (this->ist4.WB[1])
				data1=this->ist4.data1;
			else
				data1=this->ist4.data2;
		}
		if (this->ist4.WB[0] && this->ist4.rd!=0 && this->ist3.rd!=this->ist2.rt && this->ist4.rd==this->ist2.rt) {
			//out("rt happen forwarding in MEM");
			if (this->reg4.WB[1])
				data2=this->ist4.data1;
			else
				data2=this->ist4.data2;
		}

		// write data
		this->reg3.wdata=data2;

		// ALU Src
		if (this->ist2.EX[3]) {
			//out("this is immediate inst");
			data2=this->ist2.imm;
		}
		
		// Reg Dst
		if (this->ist2.EX[0]) {
			//out("Reg Dst is 1 : R format");
			this->reg3.rd=this->ist2.rd;
		}
		else {
			//out("Reg Dst is 0 : lw instrance");
			this->reg3.rd=this->ist2.rt;
		}

		int result=0;
		//cout << "EX[1]: " << ist2.EX[1] << " EX[2]: " << ist2.EX[2] << endl;
		if (this->ist2.EX[1]==0 && this->ist2.EX[2]==0) {
			result=data1+data2;
			//cout << "lw  data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
		}
		else if (this->ist2.EX[1]==0 && this->ist2.EX[2]==1) {
			result=data1-data2;
			//cout << "beq  data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
		}
		else {
			int function=(this->ist2.imm)%64;
			if (function==32) {
				result=data1+data2;
				//cout << "     add data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
			}
			else if (function==34) {
				result=data1-data2;
				//cout << "     sub data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
			}
			else if (function==36) {
				result=fun(0, data1, data2);
				//cout << "     and data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
			}
			else if (function==37) {
				result=fun(1, data1, data2);
				//cout << "     or  data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
			}
			else if (function==42) {
				if (data1<data2)
					result=1;
				else
					result=0;
				//cout << "     slt data1: " << data1 << " data2: " << data2 << " result: " << result << endl;
			}
			else if (function==0) {
				result=data1*pow(2, this->ist2.imm);
				//cout << "     sll data1: " << data1 << " imm: " << this->ist2.imm << " result: " << result << endl;
			}
		}
		this->reg3.result=result;
	}
	void MEM() {
		//cout << endl << "-----     MEM state     -----" << endl;
		this->reg4.pass(&(this->ist3));
		this->reg4.data2=this->ist3.result;
		if (this->ist3.MEM[1]) {
			this->reg4.data1=this->mem.get(this->ist3.result);
			//out("this is memery read");
		}
		else if (this->ist3.MEM[2]) {
			this->mem.set(this->ist3.result, this->ist3.wdata);
			//out("this is memory write");
		}
		this->reg4.rd=this->ist3.rd;
		//cout << "rd: " << reg4.rd << endl;
	}
	void WB() {
		//cout << endl << "-----     WB state     -----" << endl;
		int data;
		if (this->ist4.WB[1]) {
			data=this->ist4.data1;
			//out("this is mem to reg");
		}
		else {
			data=this->ist4.data2;
			//out("this is not mem to reg");
		}
		if (this->ist4.WB[0]) {
			this->reg.set(this->ist4.rd, data);
			//cout << "set $" << ist4.rd << " as " << data << endl;
		}
	}
	/*
	void out(const char *s) {
		cout << "     " << s << endl;
	}
	*/
	void pass_register() {
		this->ist1.copy(&reg1);
		//cout << "pass: EX[1]: " << reg2.EX[1] << " EX[2]: " << reg2.EX[2] << endl;
		//cout << "pass: EX[1]: " << ist2.EX[1] << " EX[2]: " << ist2.EX[2] << endl;
		this->ist2.copy(&reg2);
		//cout << "pass: EX[1]: " << reg2.EX[1] << " EX[2]: " << reg2.EX[2] << endl;
		//cout << "pass: EX[1]: " << ist2.EX[1] << " EX[2]: " << ist2.EX[2] << endl;
		this->ist3.copy(&reg3);
		this->ist4.copy(&reg4);
	}
	int fun(int fun, int a, int b) {//fun=0 and , fun=1 or
		int result=0;
		int r1=a, r2=b;
		for (int i=0;i<6;i++) {
			if (fun==0)
				result=result+( (r1%2) & (r2%2) )*pow(2, i);
			else
				result=result+( (r1%2) | (r2%2) )*pow(2, i);
			//cout << "r1: " << r1 << " rs: " << r2 << " result: " << result << endl;
			r1=r1/2;
			r2=r2/2;
		}
		return result;
	}
public:
	Pipeline() {
		this->empty=Instruction();
		this->reg=Register();
		this->mem=Memory();
		this->reg1=Reg1();
		this->reg2=Reg2();
		this->reg3=Reg3();
		this->reg4=Reg4();
		this->size=0;
		this->cc=0;
		this->pc=0;
		this->spical=0;
	}
	void setInstruction(int i, char *s) {
		this->inst[i]=Instruction(s);
		this->size++;
	}
	void run(fstream &file, int screan) {
		while (this->pc<this->size+4) {
			//cout << "=======================================" << endl;
			this->IF();
			this->WB();
			this->ID();
			this->EX();
			this->MEM();
			this->pass_register();
			this->cc++;
			if (screan)
				this->show_status();
			else
				this->show_status_file(file);
			//reg.show();
			//mem.show();
		}
	}
	void show_status() {
		cout << "CC " << this->cc << ":" << endl;
		cout << endl;
		cout << this->reg.show();
		cout << endl;
		cout << this->mem.show();
		cout << endl;
		cout << "IF/ID :" << endl;
		cout << "pc\t\t" << 4*(this->pc) << endl;
		cout << "Instruction\t" << this->ist1.inst->get_bit_string() << endl;
		cout << "\t\t";
		//cout << this->ist1.inst->toString();
		cout << endl;
		cout << "ID/EX :" << endl;
		cout << "ReadData1\t" << this->ist2.data1 << endl;
		cout << "ReadData2\t" << this->ist2.data2 << endl;
		if (ist2.MEM[0] || ist2.MEM[1] || ist2.MEM[2])
			cout << "sign_ext\t" << this->ist2.imm << endl;
		else
			cout << "sign_ext\t0" << endl;
		cout << "Rs\t\t" << this->ist2.rs << endl;
		cout << "Rt\t\t" << this->ist2.rt << endl;
		cout << "Rd\t\t" << this->ist2.rd << endl;
		cout << "Control signals\t" << this->ist2.get_control() << endl;
		cout << endl;
		cout << "EX/MEM :" << endl;
		cout << "ALUout\t\t" << this->ist3.result << endl;
		cout << "WriteData\t" << this->ist3.wdata << endl;
		cout << "Rt\t\t" << this->ist3.rd << endl;
		cout << "Control signals\t" << this->ist3.get_control() << endl;
		cout << endl;
		cout << "MEM/WB :" << endl;
		cout << "ReadData\t" <<  this->ist4.data1 << endl;
		cout << "ALUout\t\t" << this->ist4.data2 << endl;
		cout << "Control signals\t" << this->ist4.get_control() << endl;
		cout << "=================================================================" << endl;
	}
	void show_status_file(fstream &file) {
		file << "CC " << this->cc << ":" << endl;
		file << endl;
		file << this->reg.show();
		file << endl;
		file << this->mem.show();
		file << endl;
		file << "IF/ID :" << endl;
		file << "pc\t\t" << 4*(this->pc) << endl;
		file << "Instruction\t" << this->ist1.inst->get_bit_string() << endl;
		file << "\t\t";
		//file << this->ist1.inst->toString();
		file << endl;
		file << "ID/EX :" << endl;
		file << "ReadData1\t" << this->ist2.data1 << endl;
		file << "ReadData2\t" << this->ist2.data2 << endl;
		if (ist2.MEM[0] || ist2.MEM[1] || ist2.MEM[2])
			cout << "sign_ext\t" << this->ist2.imm << endl;
		else
			cout << "sign_ext\t0" << endl;
		file << "Rs\t\t" << this->ist2.rs << endl;
		file << "Rt\t\t" << this->ist2.rt << endl;
		file << "Rd\t\t" << this->ist2.rd << endl;
		file << "Control signals\t" << this->ist2.get_control() << endl;
		file << endl;
		file << "EX/MEM :" << endl;
		file << "ALUout\t\t" << this->ist3.result << endl;
		file << "WriteData\t" << this->ist3.wdata << endl;
		file << "Rt\t\t" << this->ist3.rd << endl;
		file << "Control signals\t" << this->ist3.get_control() << endl;
		file << endl;
		file << "MEM/WB :" << endl;
		file << "ReadData\t" <<  this->ist4.data1 << endl;
		file << "ALUout\t\t" << this->ist4.data2 << endl;
		file << "Control signals\t" << this->ist4.get_control() << endl;
		file << "=================================================================" << endl;
	}
	void show_instruction() {
		for (int i=0;i<this->size;i++) {
			this->inst[i].toString();
		}
	}
};
void pipeline_run(int number, int screen) {
	fstream file_in;
	fstream file_out;
	string name;
	if (number==1) {
		name=string("General");
		file_in.open("General.txt", ios::in);
		if (screen==0)
			file_out.open("genResult.txt", ios::out);
	}
	else if (number==2) {
		name=string("Datahazard");
		file_in.open("Datahazard.txt", ios::in);
		if (screen==0)
			file_out.open("dataResult.txt", ios::out);
	}
	else if (number==3) {
		name=string("Lwhazard");
		file_in.open("Lwhazard.txt", ios::in);
		if (screen==0)
			file_out.open("loadResult.txt", ios::out);
	}
	else if (number==4) {
		name=string("Branchazard");
		file_in.open("Branchazard.txt", ios::in);
		if (screen==0)
			file_out.open("branchResult.txt", ios::out);
	}
	else if (number==5) {
		name=string("Branchbonus");
		file_in.open("Branchbonus.txt", ios::in);
		if (screen==0)
			file_out.open("bonusResult.txt", ios::out);
	}

	if (number>0 && number<6) {
		char input[200];
		Pipeline pip=Pipeline();

		if (!file_in) {
			cout << "Don't exist file name " << name << " !" << endl;
		}
		else {
			int n=0;
			while (!file_in.eof()) {
				file_in.getline(input, sizeof(input));
				Instruction inst=Instruction(input);
				if (inst.be_correct()){
					pip.setInstruction(n, input);
					n++;
				}
			}
		}
		file_in.close();
		//pip.show_instruction();
		pip.run(file_out, screen);
	
		if (screen==0)
			file_out.close();
	}
}
void choose(const char *message, int screen) {
	string name(message);
	int type=-1; // 1 is file , 2 is all
	fstream file;
	if (name==string("General"))
		type=1;
	else if (name==string("Datahazard"))
		type=2;
	else if (name==string("Lwhazard"))
		type=3;
	else if (name==string("Branchazard"))
		type=4;
	else if (name==string("Branchbonus"))
		type=5;
	else if (name==string("All"))
		type=6;
	if (type==-1)
		cout << "Input error!" << endl;
	else if (type>0 && type<6)
		pipeline_run(type, screen);
	else if (type==6) {
		for (int i=1;i<6;i++) {
			pipeline_run(i, screen);
		}
	}
}
int main(int argc, char* argv[]) {
	if (argc>3)
		cout << "Error\nPlease input \"./a.out\"\nor \"./a.out fileName\"\n-s is output on screen" << endl;
	if (argc==3) {
		if (argv[1][0]=='-' && argv[1][1]=='s')
			choose(argv[2], 1);
	}
	else if (argc==2) {
		if (argv[1][0]=='-' && argv[1][1]=='s')
			choose("All", 1);
		else
			choose(argv[1], 0);
	}
	else {
		choose("All", 0);
	}
	
	
	return 0;
}
