# setup project root dir
set(CCOS_DIR ${CMAKE_CURRENT_LIST_DIR}/.. CACHE INTERNAL "")
set(CCOS_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR})
set(THIRDPARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty)

if(NOT EXISTS ${CCOS_CONFIG_DIR}/CcBuildConfig/CcBuildConfig.cmake)
  execute_process(COMMAND git submodule init "${CCOS_CONFIG_DIR}/CcBuildConfig"
          WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
  execute_process(COMMAND git submodule update "${CCOS_CONFIG_DIR}/CcBuildConfig"
          WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
include(${CCOS_CONFIG_DIR}/CcBuildConfig/CcBuildConfig.cmake)

################################################################################
# Load Macros from CcOS
################################################################################
include(${CCOS_CONFIG_DIR}/ProjectMacros.cmake )

CcAddProject(CcTestModule             Modules/CcTestModule    
                SYSTEM Windows Linux          
                DEPENDS CcKernel
)
CcAddProject(openssl                  ThirdParty/openssl      
                SYSTEM Windows Linux
)
CcAddProject(jpeg                     ThirdParty/jpeg      
                SYSTEM Windows Linux Generic
)
CcAddProject(mtp                      ThirdParty/mtp          
                SYSTEM Linux
)
CcAddProject(sqlite3                  ThirdParty/sqlite3      
                SYSTEM Windows Linux
)
CcAddProject(wiringPi                 ThirdParty/wiringPi     
                SYSTEM Linux
)
CcAddProject(CcAppInterfaces          CcAppInterfaces         
                SYSTEM Windows Linux 
                DEPENDS CcKernel
)
CcAddProject(CcBitcoin                CcBitcoin               
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel CcHttp CcDocuments
)
CcAddProject(CcDhcp                   CcDhcp                  
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel CcDocuments
)
CcAddProject(CcDocuments              CcDocuments             
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcFtp                    CcFtp                   
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcHttp                   CcHttp                  
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel 
                OPTIONAL CcSsl
)
CcAddProject(CcKernel                 CcKernel                
                SYSTEM Windows Linux Generic
)
CcAddProject(CcKernelModule           CcKernelModule          
                SYSTEM Windows Linux
)

CcAddProject(CcKernelModuleInterface  CcKernelModuleInterface 
                SYSTEM Windows Linux
)
CcAddProject(CcMedia                  CcMedia                 
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel 
                OPTIONAL CcHttp
)
CcAddProject(CcModule                 CcModule                
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcMqtt                   CcMqtt                  
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcMtp                    CcMtp                   
                SYSTEM Linux
                DEPENDS CcKernel
)
CcAddProject(CcRemoteDevice           CcRemoteDevice          
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel CcDocuments CcHttp
)
CcAddProject(CcScpi                   CcScpi                  
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcShell                  CcShell                 
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcSql                    CcSql                   
                SYSTEM Windows Linux Generic    
                DEPENDS CcKernel
                OPTIONAL sqlite3
)
CcAddProject(CcSsl                    CcSsl                   
                SYSTEM Windows Linux         
                DEPENDS CcKernel CcModule
)
CcAddProject(CcTesting                CcTesting               
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel CcModule
)
CcAddProject(CcTftp                   CcTftp                  
                SYSTEM Windows Linux Generic 
                DEPENDS CcKernel
)
CcAddProject(CcConvert                Apps/CcConvert                
                SYSTEM Windows Linux    
                DEPENDS CcKernel CcMedia
)
CcAddProject(CcDhcpServer             Apps/CcDhcpServer             
                SYSTEM Windows Linux    
                DEPENDS CcKernel CcDocuments CcDhcp
)
CcAddProject(CcFs                     Apps/CcFs                     
                SYSTEM Windows Linux    
                DEPENDS CcKernelModule
)
CcAddProject(CcHash                   Apps/CcHash                   
                SYSTEM Windows Linux    
                DEPENDS CcKernel
)
CcAddProject(CcKernelModuleTestDriver Apps/CcKernelModuleTestDriver 
                SYSTEM Windows Linux    
                DEPENDS CcKernelModule
)
CcAddProject(CcOSArgumentsApp         Apps/CcOSArgumentsApp         
                SYSTEM Windows Linux    
                DEPENDS CcKernel
)
CcAddProject(CcOSModuleLoader         Apps/CcOSModuleLoader         
                SYSTEM Windows Linux    
                DEPENDS CcKernel
)
CcAddProject(CcOSResourceApp          Apps/CcOSResourceApp          
                SYSTEM Windows Linux    
                DEPENDS CcKernel
)
CcAddProject(CcRemoteDeviceClient     Apps/CcRemoteDeviceClient     
                SYSTEM Windows Linux    
                DEPENDS CcKernel CcDocuments
)
CcAddProject(CcRemoteDeviceServer     Apps/CcRemoteDeviceServer     
                SYSTEM Windows Linux    
                DEPENDS CcKernel CcDocuments
)
CcAddProject(CcScpiCli                Apps/CcScpiCli                
                SYSTEM Windows Linux    
                DEPENDS CcKernel
)
CcAddProject(CcShellApp               Apps/CcShellApp               
                SYSTEM Windows Linux    
                DEPENDS CcKernel
)
CcAddProject(CcUsbDriver              Apps/CcUsbDriver              
                SYSTEM Windows Linux    
                DEPENDS CcKernelModule
)
CcAddProject(CcVDisk                  Apps/CcVDisk                  
                SYSTEM Windows Linux    
                DEPENDS CcKernelModule
)
