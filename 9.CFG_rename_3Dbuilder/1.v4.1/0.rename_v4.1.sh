#!/bin/bash

cat input.dat | sed 's/(/\[/g;
	s/)/\]/g;
	s/-b-/b-/g;
	s/-Glc/Glc/g;
	s/-Gal/Gal/g;
	s/Fuc/LFucp/g;
	s/Gal/DGalp/g;
	s/Glc/DGlcp/g;
	s/Man/DManp/g;
	s/Rha/LRhap/g;
	s/Neu5Ac-/DNeup5Ac2-/g;
        s/Neu5Gc-/DNeup5Gc2-/g;
	s/Neu5Ac/DNeup5Ac/g;
	s/Neu5Gc/DNeup5Gc/g;
	s/Nac/NAc/g;
	s/N?c/NAc/g;
	s/sp/Sp/g;
	s/SP/Sp/g;
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
	s/\ //g;' | grep -v "9Ac" | grep -v "9NAc" | grep -v "Mur" | grep -v "Lys" | grep -v "KDN" | grep -v "ola" | grep -v "6P" | grep -v "\[Gc\]" | grep -v "H2PO3" | grep -v "NH2" | grep -v "AVL" | grep -v "NKT" | grep -v "CH3" | grep -v "G-ol" > output0.txt
