function(platform_specific_include prefix suffix output)
  file(GENERATE
    OUTPUT ${output}
    CONTENT "#pragma once\n\n#include <${prefix}$<IF:${WIN32},windows,linux>${suffix}>"
  )
endfunction()
