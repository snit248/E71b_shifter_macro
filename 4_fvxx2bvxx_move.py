# reading modules
import py_subprocess
import os
import sys
import py_set_path
import time
import py_compress
import glob
import shutil
import datetime

# def fuction name(arugument 1, 2, ...):
def dc_thick(path,pl,area,zone):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    exe_path=r"dc.exe {0:0=3}-{1} --o dc-{0:0=3} --view 2000 500 --rc ../../rc/dc_new.rc --descriptor ..\..\EventDescriptor-0.ini --search-mode 0".format(pl,zone)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    exe_path=r"root -q -l -b M:\data\test\work\tom\prg\T60\ROOT\PlotDcT60.C(\"dc-{0:0=3}.lst\")".format(pl)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def m2b_thick(path,pl,area,zone):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    exe_path=r"m2b.exe {0:0=3}-{1} --view 2000 1300 --rc ../../rc/m2b_re.rc --descriptor ..\..\EventDescriptor-0.ini --c dc-{0:0=3}.lst".format(pl,zone)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def m2b_thin(path,pl,area,zone):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    exe_path=r"m2b.exe {0:0=3}-{1} --view 2000 1300 --rc ../../rc/m2b_re_thin.rc --descriptor ..\..\EventDescriptor-0.ini --c dc-{0:0=3}.lst".format(pl,zone)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True,timeout=4*60*60)
    if ret != 0:raise Exception

def rankingcut_thick(path,pl,area,num):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    if not os.path.isfile(target_dir+r"\param3.txt"):
        #ret = py_subprocess.run(r"copy Q:\RUN9\param\param3.txt "+target_dir,target_dir,silent_cout=True)
        ret = py_subprocess.run(r"copy D:\Analysis\NINJA\RUN9\rankingcut\param\param3.txt "+target_dir,target_dir,silent_cout=True)
        if ret != 0:raise Exception
    if not os.path.isfile(target_dir+r"\param3_lat.txt"):
        #ret = py_subprocess.run(r"copy Q:\RUN9\param\param3_lat.txt "+target_dir,target_dir,silent_cout=True)
        ret = py_subprocess.run(r"copy D:\Analysis\NINJA\RUN9\rankingcut\param\param3_lat.txt "+target_dir,target_dir,silent_cout=True)
        if ret != 0:raise Exception
    exe_path=r"M:\data\NINJA\prg\Basetrack_Rankingcut_v2.exe b{0:0=3}_thick_{1}.vxx {0:0=3} 0 b{0:0=3}_thick_{1}.sel.vxx param3.txt param3_lat.txt".format(pl,num)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def rankingcut_thin(path,pl,area,num):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    if not os.path.isfile(target_dir+r"\param3_thin.txt"):
        #ret = py_subprocess.run(r"copy Q:\RUN9\param\param3_thin.txt "+target_dir,target_dir,silent_cout=True)
        ret = py_subprocess.run(r"copy D:\Analysis\NINJA\RUN9\rankingcut\param\param3_thin.txt "+target_dir,target_dir,silent_cout=True)
        if ret != 0:raise Exception
    if not os.path.isfile(target_dir+r"\param3_thin_lat.txt"):
        #ret = py_subprocess.run(r"copy Q:\RUN9\param\param3_thin_lat.txt "+target_dir,target_dir,silent_cout=True)
        ret = py_subprocess.run(r"copy D:\Analysis\NINJA\RUN9\rankingcut\param\param3_thin_lat.txt "+target_dir,target_dir,silent_cout=True)
        if ret != 0:raise Exception
    exe_path=r"M:\data\NINJA\prg\Basetrack_Rankingcut_v2.exe b{0:0=3}_thin_{1}.vxx {0:0=3} 0 b{0:0=3}_thin_{1}.sel.vxx param3_thin.txt param3_thin_lat.txt".format(pl,num)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def draw_basetrack(path,pl,area,bvxx):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    exe_path=r"dump_bvxx "+bvxx+".vxx {0:0=3} 0 --format 1 > tmp.dmp".format(pl)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    exe_path=r"root -b -q -l e:\udd\graine\prg\B15-01\PlotTools\v1\BvxxBinary2RootCompOff\BvxxBinary2Root.dll(\""+bvxx+r".root\") < tmp.dmp"
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    exe_path=r"root -b -q -l M:\data\NINJA\ROOT\PlotBtT81_lateral_LA.C(\""+bvxx+r".root\")"
    proc = py_subprocess.run_nowait(exe_path,target_dir)
    if ret != 0:raise Exception
    #exe_path="magick convert -density 150 PlotBt_{0}.root-0.ps -rotate 90 PlotBt_{0}.pdf".format(bvxx)
    #proc = py_subprocess.run_nowait(exe_path,target_dir)

    os.remove(target_dir+r"\tmp.dmp")

    return proc
    
def merge4C3(path,pl,area):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)

    exe_path=r"M:\data\NINJA\prg\Basetrack_4C3_selection.exe b{0:0=3}_thick_0.sel.vxx b{0:0=3}_thick_1.sel.vxx b{0:0=3}_thick_2.sel.vxx b{0:0=3}_thick_3.sel.vxx {0} b{0:0=3}_thick.vxx".format(pl)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

    exe_path=r"M:\data\NINJA\prg\Basetrack_4C3_selection.exe b{0:0=3}_thin_0.sel.vxx b{0:0=3}_thin_1.sel.vxx b{0:0=3}_thin_2.sel.vxx b{0:0=3}_thin_3.sel.vxx {0} b{0:0=3}_thin.vxx".format(pl)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def ghost_filter(path,pl,area):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    exe_path=r"M:\data\NINJA\prg\Basetrack_ghost_filter_thick_thin.exe b{0:0=3}_thick.vxx b{0:0=3}_thin.vxx {0} b{0:0=3}.sel.vxx".format(pl)

    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
def bvxx_correction(path,pl,area):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl)
    exe_path=r"M:\data\NINJA\prg\Basetrack_correction.exe b{0:0=3}.sel.vxx {0} {1} b{0:0=3}.sel.cor.vxx".format(pl,area)

    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception


def fvxx_PH_cut(path,pl,area,ph):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    tmp_file=target_dir+r"\tmp.vxx"

    ##pos1 thick0
    target_file="f{0:0=3}1_thick_0.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}1 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    ##pos1 thick1
    target_file="f{0:0=3}1_thick_1.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}1 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    ##pos2 thick0
    target_file="f{0:0=3}2_thick_0.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}2 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    ##pos2 thick1
    target_file="f{0:0=3}2_thick_1.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}2 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

    ##pos1 thin0
    target_file="f{0:0=3}1_thin_0.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}1 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    ##pos1 thin1
    target_file="f{0:0=3}1_thin_1.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}1 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    ##pos2 thin0
    target_file="f{0:0=3}2_thin_0.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}2 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    ##pos2 thin1
    target_file="f{0:0=3}2_thin_1.vxx".format(pl)
    target_file_full=target_dir+"\\"+target_file
    if os.path.isfile(tmp_file):
        os.remove(tmp_file)
    os.rename(target_file_full,tmp_file)
    exe_path="f_filter {0}2 tmp.vxx --o ".format(pl)+target_file+ " --ph 1 100 {0} --view 10000 1000".format(ph)
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

    if os.path.isfile(tmp_file):
        os.remove(tmp_file)

def pick_fvxx_inf(path,pl,area,num):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    prg_path=r"m:\data\NINJA\prg\basetrack_pick_microtrack_inf.exe"

    bvxx_thick="b{0:0=3}_thick_{1}.sel.vxx".format(pl,num)
    bvxx_thin="b{0:0=3}_thin_{1}.sel.vxx".format(pl,num)
    out_bvxx_thick="micro_inf_thick_{0}".format(num)
    out_bvxx_thin="micro_inf_thin_{0}".format(num)
    exe_path=prg_path+" "+bvxx_thick+" {0} {1} ".format(pl,area)+target_dir+" "+out_bvxx_thick
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception
    exe_path=prg_path+" "+bvxx_thin+" {0} {1} ".format(pl,area)+target_dir+" "+out_bvxx_thin
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def pick_fvxx_inf_all(path,pl,area):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    prg_path=r"m:\data\NINJA\prg\basetrack_pick_microtrack_inf_all_v2.exe"
    #usage:prg in-bvxx-folder pl area fvxx-folder out-file-path
    exe_path=prg_path+" "+target_dir+" {0} {1} ".format(pl,area)+target_dir+" "+target_dir
    ret = py_subprocess.run(exe_path,target_dir,silent_cout=True)
    if ret != 0:raise Exception

def compress_fvxx_bvxx(path,pl,area):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    fvxx = glob.glob(f"{target_dir}/f*.vxx")
    bvxx = glob.glob(f"{target_dir}/b*_*.vxx")
    
    files = fvxx+bvxx

    for i, file in enumerate(files):
        text = "{} {}".format(file,py_compress.calc_crc32(file))
        py_compress.compress_7z(file)
        print(text,"{} / {}".format(i + 1,len(files)),end="                    \r")
        with open(os.path.join(target_dir,"large_size_files.txt"),"a") as f:
            f.write(text + "\n")
    print()
def copy_data(input_path,output_path,pl,area):
    input_dir=input_path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    output_dir=output_path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    #input --> output
    exe_command="robocopy "+input_dir+" "+output_dir+" /E /xd DATA"
    ret = py_subprocess.run(exe_command,input_path,silent_cout=False)
    print("robocopy error level {0}".format(ret))
    if ret >7:raise Exception

def delete_dir_no_wait(path):
    exe_command="rmdir /s /q "+path
    current_dir=path+r"\.."
    ret = py_subprocess.run_nowait(exe_command,current_dir)

def delete_data(path,pl,area):
    target_dir=path+r"\Area{0}\PL{1:0=3}".format(area,pl) 
    delete_dir_no_wait(target_dir)


if __name__ == '__main__':
    if len(sys.argv) !=2:
        print("argument not found\n")
        raise Exception
    read_file=sys.argv[1]

    time_log_file=""

    input_path=""
    output_path=""
    pl=[]
    area=[]


    with open(read_file,"r") as f:
        data=f.read()

    lines=data.split('\n')
    for num in range(len(lines)):
        line=lines[num].split()
        if num==0 and len(line)==1:
            input_path=line[0]
        elif num==1 and len(line)==1:
            output_path=line[0]
        elif num==2 and len(line)==1:
            time_log_file=line[0]
        elif num==0 or num==1 or num==2:
            print("except file format")
            print(":::::::::::::::::::::::::")
            print(r"input file  [I:\NINJA\E71a\ECC5]")
            print(r"output file  [I:\NINJA\E71a\ECC5]")
            print(r"time log file  [I:\NINJA\E71a\work\suzuki\time\ECC6_make_bvxx.txt]")
            print("[pl] [area]")
            print(":::::::::::::::::::::::::")
            raise Exception
        else:
            if len(line) != 2:
                print("except file format\n")
                print("content:",line)
            if len(line) == 2:
                pl.append(int(line[0]))
                area.append(int(line[1]))
    print("path:"+input_path)
    spath = py_set_path.SetPath(r"I:\prg\python\python_app\python_app\my_path.yml")
    spath.set_netscan_path()

    for num in range(len(pl)):
        #開始時間記録
        start = time.time()
        proc=[]
        #dc_thick(input_path,pl[num],area[num],area[num])
        for num2 in range(4):
            zone=num2*6+area[num]
            #m2b_thick(input_path,pl[num],area[num],zone)
            rankingcut_thick(input_path,pl[num],area[num],num2)
            bvxx_thick="b{0:0=3}_thick_{1}.sel".format(pl[num],num2)
            proc.append(draw_basetrack(input_path,pl[num],area[num],bvxx_thick))

            zone=num2*6+area[num]+4*6
            #m2b_thin(input_path,pl[num],area[num],zone)
            rankingcut_thin(input_path,pl[num],area[num],num2)
            bvxx_thin="b{0:0=3}_thin_{1}.sel".format(pl[num],num2)
            proc.append(draw_basetrack(input_path,pl[num],area[num],bvxx_thin))

        #4C3 selection small&large
        merge4C3(input_path,pl[num],area[num])
        #merge&ghost消し
        ghost_filter(input_path,pl[num],area[num])
        #絵を描く
        bvxx_name="b{0:0=3}_thick".format(pl[num])
        proc.append(draw_basetrack(input_path,pl[num],area[num],bvxx_name))
        bvxx_name="b{0:0=3}_thin".format(pl[num])
        proc.append(draw_basetrack(input_path,pl[num],area[num],bvxx_name))
        bvxx_name="b{0:0=3}.sel".format(pl[num])
        proc.append(draw_basetrack(input_path,pl[num],area[num],bvxx_name))
        #0.951 correction
        bvxx_correction(input_path,pl[num],area[num])
        #描画の終了チェック
        for check_num in range(len(proc)):
            ret=py_subprocess.wait_process(proc[check_num],output=False)
            if ret != 0:raise Exception
        #終了時間記録
        elapsed_time = time.time() - start
        dt_now = datetime.datetime.now()
        with open(time_log_file,"a") as f:
            f.write("{0:0=3} {1:0=1} {2} {3} {4} {5} {6} {7} {8:.1f}\n".format(pl[num],area[num],dt_now.year,dt_now.month,dt_now.day,dt_now.hour,dt_now.minute,dt_now.second,elapsed_time))       


        #ここから圧縮&移動
        #basetrackになっているmicrotrackのhit num情報出力
        #pick_fvxx_inf(input_path,pl[num],area[num],0)
        #pick_fvxx_inf(input_path,pl[num],area[num],1)
        #pick_fvxx_inf(input_path,pl[num],area[num],2)
        #pick_fvxx_inf(input_path,pl[num],area[num],3)
        #pick_fvxx_inf_all(input_path,pl[num],area[num])

        #fvxxのPH cut
        #fvxx_PH_cut(input_path,pl[num],area[num],9)

        #fvxxの圧縮
        #bvxx merge以外の圧縮
        #compress_fvxx_bvxx(input_path,pl[num],area[num])
        #copy_data(input_path,output_path,pl[num],area[num])
        #delete_data(input_path,pl[num],area[num])

    # 最後にpathをもとに戻す
    spath.reset_path()
