#include "/home/oliver/Programs/GEMS-July2016/gmml/includes/gmml.hpp"
#include <string>


using namespace MolecularModeling;

int main(int argc, char *argv[])
{
    //************************************************//
    // Details for creating in a PDB file              //
    //************************************************//

    std::vector<std::string> amino_libs, glycam_libs, other_libs, prep;
    amino_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/amino12.lib");
    amino_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/aminoct12.lib");
    amino_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/aminont12.lib");

    glycam_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_amino_06j_12SB.lib");
    glycam_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_aminoct_06j_12SB.lib");
    glycam_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_aminont_06j_12SB.lib");

    other_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/nucleic12.lib");
    other_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc.ff12SB_2014-04-24/nucleic12.lib");
    other_libs.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/other/solvents.lib");

    prep.push_back("/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_06j-1.prep");

    std::string parameter_file_path = "/home/oliver/Programs/GEMS-July2016/gmml/dat/CurrentParams/leaprc_GLYCAM_06j-1_2014-03-14/GLYCAM_06j.dat";

    //************************************************//
    // Build the PDB file and save it                  //
    //************************************************//

    Assembly assemblyC;
    assemblyC.BuildAssemblyFromCondensedSequence("DGalpa1-4[DGlcpb1-2]DManpa1-2DManpa1-OH", prep.at(0) ,parameter_file_path, true);
    PdbFileSpace::PdbFile *outputPdbFile1 = assemblyC.BuildPdbFileStructureFromAssembly();
    outputPdbFile1->Write("/home/oliver/Desktop/test-BuildFromSequence.pdb");

    return 0;
}
