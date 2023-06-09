#!/bin/bash

cat input.dat | sed 's/(/\[/g;
	s/)/\]/g;
	s/α/a/g;
	s/β/b/g;
	s/–/-/g;
	s/a-D-Gal/Gala/g;
	s/b-D-Gal/Galb/g;
	s/a-D-Glc/Glca/g;
	s/b-D-Glc/Glcb/g;
	s/a-D-Man/Mana/g;
	s/b-D-Man/Manb/g;
	s/a-GalNAc/GalNAca/g;
	s/a-L-Fuc/Fuca/g;
	s/a-L-Rha/Rhaa/g;
	s/a-Neu5Ac/Neu5Aca/g;
	s/b-Neu5Ac/Neu5Acb/g;
	s/b-GalNAc/GalNAcb/g;
	s/b-GlcNAc/GlcNAcb/g;
	s/-b-/b-/g;
	s/-Glc/Glc/g;
	s/-Gal/Gal/g;
	s/Fuc/LFucp/g;
	s/Gal/DGalp/g;
	s/Glc/DGlcp/g;
	s/Man/DManp/g;
	s/Rha/LRhap/g;
	s/NeuAc/Neu5Ac/g; #assuming typo and not 9Ac.
        s/Neu5Aa/Neu5Aca/g;
	s/Neu5Ac-/DNeup5Ac2-/g;
        s/Neu5Gc-/DNeup5Gc2-/g;
	s/Neu5Ac/DNeup5Ac/g;
	s/Neu5Gc/DNeup5Gc/g;
	s/Nac/NAc/g;
	s/N?c/NAc/g;
	s/SpSp/Sp/g;
	s/sp/Sp/g;
	s/SP/Sp/g;
        s/ Sp/-Sp/g;
	s/-Sp10/1-OH/g;
	s/-Sp11/1-OH/g;
	s/-Sp12/1-OH/g;
	s/-Sp13/1-OH/g;
	s/-Sp14/1-OH/g;
	s/-Sp15/1-OH/g;
	s/-Sp16/1-OH/g;
	s/-Sp17/1-OH/g;
	s/-Sp18/1-OH/g;
	s/-Sp19/1-OH/g;
	s/-Sp20/1-OH/g;
	s/-Sp21/1-OH/g;
	s/-Sp22/1-OH/g;
	s/-Sp0/1-OH/g;
        s/-Sp1/1-OH/g;
        s/-Sp8/1-OH/g;
        s/-Sp9/1-OH/g;
	s/6-O-Su/6S/g;
	s/2OSO3/2S/g;
	s/3OSO3/3S/g;
        s/4OSO3/4S/g;
        s/6OSO3/6S/g;
	s/\t2S/\t\[2S\]/g;
        s/\t3S/\t\[3S\]/g;
        s/\t4S/\t\[4S\]/g;
        s/\t6S/\t\[6S\]/g;	
	s/DNeup5Aca1-OH/DNeup5Aca2-OH/g;
        s/DNeup5Acb1-OH/DNeup5Acb2-OH/g;
        s/DNeup5Gca1-OH/DNeup5Gca2-OH/g;
        s/DNeup5Gca1-OH/DNeup5Gca2-OH/g;
	s/GlcpNAb/GlcpNAcb/g; #assumng typo
	s/GlcpNAa/GlcpNAca/g; #assumng typo
	s/DManpa2/DManpa1-2/g;
	s/-3-/-3/g;
	s/-2-/-2/g;	
	s/\ //g;' | grep -v "9Ac" | grep -v "9NAc" | grep -v "Mur" | grep -v "Lys" | grep -v "KDN" | grep -v "ola" | grep -v "6P" | grep -v "\[Gc\]" | grep -v "H2PO3" | grep -v "NH2" | grep -v "AVL" | grep -v "NKT" | grep -v "CH3" | grep -v "G-ol" | grep -v "NAcb1-4\]6b1" | grep -v "4\]5" | grep -v "," > output0.txt
