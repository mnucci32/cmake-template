find_program(PDFLATEX_COMPILER pdflatex REQUIRED)
find_program(BIBTEX_COMPILER bibtex REQUIRED)

if(NOT PDFLATEX_COMPILER)
  message(FATAL_ERROR "pdflatex not found")
endif()
if(NOT BIBTEX_COMPILER)
  message(FATAL_ERROR "bibtex not found")
endif()

message(STATUS "PDFLATEX_COMPILER: ${PDFLATEX_COMPILER}")
message(STATUS "BIBTEX_COMPILER: ${BIBTEX_COMPILER}")

# specify the name of the main .tex file
set(MAIN_TEX_FILE "user_guide")

# define LaTeX source files
set(LATEX_SOURCES ${MAIN_TEX_FILE}.tex references.bib)

# add custom command to build the PDF
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_TEX_FILE}.pdf
  COMMAND ${PDFLATEX_COMPILER} -interaction=nonstopmode -output-directory ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_TEX_FILE}.tex
  COMMAND ${PDFLATEX_COMPILER} -interaction=nonstopmode -output-directory ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${MAIN_TEX_FILE}.tex
  DEPENDS ${LATEX_SOURCES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Building LaTeX PDF"
)

# add custom target to build the PDF
add_custom_target(user_guide ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_TEX_FILE}.pdf)

# install the PDF
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${MAIN_TEX_FILE}.pdf DESTINATION doc)