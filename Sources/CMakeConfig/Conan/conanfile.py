from conans import ConanFile, CMake

class CcOS(ConanFile):
   settings = "os", "compiler", "build_type", "arch"
   requires = "openssl/1.1.1n" # comma-separated list of requirements
   generators = "cmake", "gcc", "txt", "visual_studio"
   default_options = {}

   def imports(self):
      self.copy("*.dll", dst="bin", src="bin") # From bin to bin
      self.copy("*.lib", dst="lib", src="lib") # From lib to bin