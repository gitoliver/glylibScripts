import cPickle, base64
try:
	from SimpleSession.versions.v58 import beginRestore,\
	    registerAfterModelsCB, reportRestoreError, checkVersion
except ImportError:
	from chimera import UserError
	raise UserError('Cannot open session that was saved in a'
	    ' newer version of Chimera; update your version')
checkVersion([1, 8, 38824])
import chimera
from chimera import replyobj
replyobj.status('Restoring session...', \
    blankAfter=0)
replyobj.status('Beginning session restore...', \
    blankAfter=0, secondary=True)
beginRestore()

def restoreCoreModels():
	from SimpleSession.versions.v58 import init, restoreViewer, \
	     restoreMolecules, restoreColors, restoreSurfaces, \
	     restoreVRML, restorePseudoBondGroups, restoreModelAssociations
	molInfo = cPickle.loads(base64.b64decode('gAJ9cQEoVRFyaWJib25JbnNpZGVDb2xvcnECSwROfYdVCWJhbGxTY2FsZXEDSwRHP9AAAAAAAAB9h1UJcG9pbnRTaXplcQRLBEc/8AAAAAAAAH2HVQVjb2xvcnEFSwRLAH1xBihLAV1xB0sBYUsCXXEISwJhSwNdcQlLA2F1h1UKcmliYm9uVHlwZXEKSwRLAH2HVQpzdGlja1NjYWxlcQtLBEc/8AAAAAAAAH2HVQxhcm9tYXRpY01vZGVxDEsESwF9h1UKdmR3RGVuc2l0eXENSwRHQBQAAAAAAAB9h1UGaGlkZGVucQ5LBIl9h1UNYXJvbWF0aWNDb2xvcnEPSwROfYdVD3JpYmJvblNtb290aGluZ3EQSwRLAH2HVQlhdXRvY2hhaW5xEUsEiH2HVQhvcHRpb25hbHESfXETVQhvcGVuZWRBc3EUiIlLBChVDVBldFJlc2N1NC5wZGJOVQNQREJxFUsBdHEWfXEXKChVDVBldFJlc2N1Mi5wZGJOaBVLAXRxGF1xGUsBYShVDVBldFJlc2N1My5wZGJOaBVLAXRxGl1xG0sCYShVDVBldFJlc2N1MS5wZGJOaBVLAXRxHF1xHUsAYXWHh3NVD2xvd2VyQ2FzZUNoYWluc3EeSwSJfYdVCWxpbmVXaWR0aHEfSwRHP/AAAAAAAAB9h1UPcmVzaWR1ZUxhYmVsUG9zcSBLBEsAfYdVBG5hbWVxIUsEWA0AAABQZXRSZXNjdTIucGRifXEiKFgNAAAAUGV0UmVzY3U0LnBkYl1xI0sDYVgNAAAAUGV0UmVzY3UxLnBkYl1xJEsAYVgNAAAAUGV0UmVzY3UzLnBkYl1xJUsCYXWHVQ9hcm9tYXRpY0Rpc3BsYXlxJksEiX2HVQ9yaWJib25TdGlmZm5lc3NxJ0sERz/pmZmZmZmafYdVCnBkYkhlYWRlcnNxKF1xKSh9cSpYBAAAAExJTktdcSsoWDgAAABMSU5LICAgICAgICAgTyAgIE9NRSAgICAgMSAgICAgICAgICAgICAgICAgQzEgIDRZQiAgICAgMnEsWDgAAABMSU5LICAgICAgICAgTzQgIDRZQiAgICAgMiAgICAgICAgICAgICAgICAgQzEgIDBZQiAgICAgM3EtZXN9cS59cS99cTBlVQNpZHNxMUsESwNLAIZ9cTIoSwJLAIZdcTNLAmFLAUsAhl1xNEsBYUsASwCGXXE1SwBhdYdVDnN1cmZhY2VPcGFjaXR5cTZLBEe/8AAAAAAAAH2HVRBhcm9tYXRpY0xpbmVUeXBlcTdLBEsCfYdVFHJpYmJvbkhpZGVzTWFpbmNoYWlucThLBIh9h1UHZGlzcGxheXE5SwSIfYd1Lg=='))
	resInfo = cPickle.loads(base64.b64decode('gAJ9cQEoVQZpbnNlcnRxAksQVQEgfYdVC2ZpbGxEaXNwbGF5cQNLEIl9h1UEbmFtZXEESxBYAwAAADRZQn1xBShYAwAAADBZQl1xBihLA0sHSwtLD2VYAwAAAEFTTl1xByhLAEsESwhLDGVYAwAAAE9NRV1xCChLAUsFSwlLDWV1h1UFY2hhaW5xCUsQWAMAAABoZXR9h1UOcmliYm9uRHJhd01vZGVxCksQSwJ9h1UCc3NxC0sQiYmGfYdVCG1vbGVjdWxlcQxLEEsAfXENKEsBTl1xDksESwSGcQ9hhksCTl1xEEsISwSGcRFhhksDTl1xEksMSwSGcRNhhnWHVQtyaWJib25Db2xvcnEUSxBLBH1xFShLA05dcRZLDUsDhnEXYYZLBU5dcRhLBUsDhnEZYYZLBk5dcRpLCUsDhnEbYYZ1h1UFbGFiZWxxHEsQWAAAAAB9h1UKbGFiZWxDb2xvcnEdSxBLBH1xHihLA05dcR9LDUsDhnEgYYZLBU5dcSFLBUsDhnEiYYZLBk5dcSNLCUsDhnEkYYZ1h1UIZmlsbE1vZGVxJUsQSwF9h1UFaXNIZXRxJksQiH2HVQtsYWJlbE9mZnNldHEnSxBOfYdVCHBvc2l0aW9ucShdcSkoSwFLAYZxKksBSwOGcStLAUsBhnEsSwFLA4ZxLUsBSwGGcS5LAUsDhnEvSwFLAYZxMEsBSwOGcTFlVQ1yaWJib25EaXNwbGF5cTJLEIl9h1UIb3B0aW9uYWxxM31VBHNzSWRxNEsQSv////99h3Uu'))
	atomInfo = cPickle.loads(base64.b64decode('gAJ9cQEoVQdyZXNpZHVlcQJLiEsGfXEDKEsETl1xBEsASwWGcQVhhksFTl1xBksFSwGGcQdhhksHTl1xCEsUSw6GcQlhhksITl1xCksiSwWGcQthhksJTl1xDEsnSwGGcQ1hhksKTl1xDksoSw6GcQ9hhksLTl1xEEs2Sw6GcRFhhksMTl1xEktESwWGcRNhhksNTl1xFEtJSwGGcRVhhksOTl1xFktKSw6GcRdhhksPTl1xGEtYSw6GcRlhhksQTl1xGktmSwWGcRthhksRTl1xHEtrSwGGcR1hhksSTl1xHktsSw6GcR9hhksTTl1xIEt6Sw6GcSFhhnWHVQh2ZHdDb2xvcnEiS4hOfYdVBG5hbWVxI0uIWAIAAABOMn1xJChYAwAAAEMyTl1xJShLEEsfSzJLQUtUS2NLdkuFZVgCAAAAQzJdcSYoSw5LHUswSz9LUkthS3RLg2VYAgAAAENCXXEnKEsBSyNLRUtnZVgCAAAAQ0FdcSgoSwJLJEtGS2hlWAIAAABDR11xKShLAEsiS0RLZmVYAQAAAE5dcSooSwNLJUtHS2llWAMAAABPMk5dcSsoSxFLIEszS0JLVUtkS3dLhmVYAgAAAEMzXXEsKEsMSxtLLks9S1BLX0tyS4FlWAIAAABDNl1xLShLCUsXSytLOUtNS1tLb0t9ZVgDAAAAQ01FXXEuKEsSSyFLNEtDS1ZLZUt4S4dlWAMAAABPRDFdcS8oSwRLJktIS2plWAMAAABORDFdcTAoSwVLJ0tJS2tlWAIAAABPNl1xMShLCksYSyxLOktOS1xLcEt+ZVgCAAAAQzFdcTIoSwZLFEsoSzZLSktYS2xLemVYAgAAAE80XXEzKEsTSxpLNUs8S1dLXkt5S4BlWAIAAABPM11xNChLDUscSy9LPktRS2BLc0uCZVgCAAAATzVdcTUoSwdLFUspSzdLS0tZS21Le2VYAgAAAEM1XXE2KEsISxZLKks4S0xLWktuS3xlWAIAAABDNF1xNyhLC0sZSy1LO0tPS11LcUt/ZXWHVQN2ZHdxOEuIiX2HVQ5zdXJmYWNlRGlzcGxheXE5S4iJfYdVBWNvbG9ycTpLiEsEfXE7KEsDXXE8KEtmS2pLa0tsS21LbktvS3BLcUtyS3NLdEt1S3ZLd0t4S3lLekt7S3xLfUt+S39LgEuBS4JLg0uES4VLhkuHZUsFXXE9KEsiSyZLJ0soSylLKksrSyxLLUsuSy9LMEsxSzJLM0s0SzVLNks3SzhLOUs6SztLPEs9Sz5LP0tAS0FLQktDZUsGXXE+KEtES0hLSUtKS0tLTEtNS05LT0tQS1FLUktTS1RLVUtWS1dLWEtZS1pLW0tcS11LXktfS2BLYUtiS2NLZEtlZXWHVQlpZGF0bVR5cGVxP0uIiX2HVQZhbHRMb2NxQEuIVQB9h1UFbGFiZWxxQUuIWAAAAAB9h1UOc3VyZmFjZU9wYWNpdHlxQkuIR7/wAAAAAAAAfYdVB2VsZW1lbnRxQ0uISwZ9cUQoSwhdcUUoSwRLB0sKSw1LEUsTSxVLGEsaSxxLIEsmSylLLEsvSzNLNUs3SzpLPEs+S0JLSEtLS05LUUtVS1dLWUtcS15LYEtkS2pLbUtwS3NLd0t5S3tLfkuAS4JLhmVLB11xRihLA0sFSw9LHkslSydLMUtAS0dLSUtTS2JLaUtrS3VLhGV1h1UKbGFiZWxDb2xvcnFHS4hLBH1xSChLA11xSShLZktqS2tLbEttS25Lb0twS3FLcktzS3RLdUt2S3dLeEt5S3pLe0t8S31Lfkt/S4BLgUuCS4NLhEuFS4ZLh2VLBV1xSihLIksmSydLKEspSypLK0ssSy1LLksvSzBLMUsySzNLNEs1SzZLN0s4SzlLOks7SzxLPUs+Sz9LQEtBS0JLQ2VLBl1xSyhLREtIS0lLSktLS0xLTUtOS09LUEtRS1JLU0tUS1VLVktXS1hLWUtaS1tLXEtdS15LX0tgS2FLYktjS2RLZWV1h1UMc3VyZmFjZUNvbG9ycUxLiEsEfXFNKEsDXXFOKEtmS2pLa0tsS21LbktvS3BLcUtyS3NLdEt1S3ZLd0t4S3lLekt7S3xLfUt+S39LgEuBS4JLg0uES4VLhkuHZUsFXXFPKEsiSyZLJ0soSylLKksrSyxLLUsuSy9LMEsxSzJLM0s0SzVLNks3SzhLOUs6SztLPEs9Sz5LP0tAS0FLQktDZUsGXXFQKEtES0hLSUtKS0tLTEtNS05LT0tQS1FLUktTS1RLVUtWS1dLWEtZS1pLW0tcS11LXktfS2BLYUtiS2NLZEtlZXWHVQ9zdXJmYWNlQ2F0ZWdvcnlxUUuIWAQAAABtYWlufYdVBnJhZGl1c3FSS4hHP/4UeuAAAAB9cVMoRz/5wo9gAAAAXXFUKEsASxBLH0siSzJLQUtES1RLY0tmS3ZLhWVHP/o9cKAAAABdcVUoSwNLBUsPSx5LJUsnSzFLQEtHS0lLU0tiS2lLa0t1S4RlRz/2uFHgAAAAXXFWKEsESxFLIEsmSzNLQktIS1VLZEtqS3dLhmVHP/dcKQAAAABdcVcoSwdLCksNSxNLFUsYSxpLHEspSyxLL0s1SzdLOks8Sz5LS0tOS1FLV0tZS1xLXktgS21LcEtzS3lLe0t+S4BLgmV1h1ULbGFiZWxPZmZzZXRxWEuITn2HVRJtaW5pbXVtTGFiZWxSYWRpdXNxWUuIRwAAAAAAAAAAfYdVCGRyYXdNb2RlcVpLiEsCfYdVCG9wdGlvbmFscVt9cVwoVQxzZXJpYWxOdW1iZXJxXYiIXXFeKEsBSwGGcV9LAUsBhnFgSwFLAYZxYUsBSwGGcWJLAUsehnFjSwFLAYZxZEsBSwGGcWVLAUsBhnFmSwFLAYZxZ0sBSx6GcWhLAUsBhnFpSwFLAYZxaksBSwGGcWtLAUsBhnFsSwFLHoZxbUsBSwGGcW5LAUsBhnFvSwFLAYZxcEsBSwGGcXFLAUsehnFyZYdVB2JmYWN0b3Jxc4iJS4hHAAAAAAAAAAB9h4dVCW9jY3VwYW5jeXF0iIlLiEc/8AAAAAAAAH2Hh3VVB2Rpc3BsYXlxdUuIiH2HdS4='))
	bondInfo = cPickle.loads(base64.b64decode('gAJ9cQEoVQVhdG9tc3ECXXEDKF1xBChLFEsVZV1xBShLFEsYZV1xBihLFUsWZV1xByhLFksXZV1xCChLGksbZV1xCShLGksiZV1xCihLG0scZV1xCyhLHEsdZV1xDChLHEsfZV1xDShLHUseZV1xDihLH0sgZV1xDyhLH0snZV1xEChLIEshZV1xEShLIEsiZV1xEihLIksjZV1xEyhLI0skZV1xFChLJEslZV1xFShLJEsmZV1xFihLGUsaZV1xFyhLKEspZV1xGChLKEsxZV1xGShLKUsqZV1xGihLKksrZV1xGyhLKkstZV1xHChLK0ssZV1xHShLLUsuZV1xHihLLUsvZV1xHyhLL0swZV1xIChLL0sxZV1xIShLMUsyZV1xIihLMkszZV1xIyhLM0s0ZV1xJChLM0s1ZV1xJShLJ0soZV1xJihLFEsZZV1xJyhLNks3ZV1xKChLNks6ZV1xKShLN0s4ZV1xKihLOEs5ZV1xKyhLPEs9ZV1xLChLPEtEZV1xLShLPUs+ZV1xLihLPks/ZV1xLyhLPktBZV1xMChLP0tAZV1xMShLQUtCZV1xMihLQUtJZV1xMyhLQktDZV1xNChLQktEZV1xNShLREtFZV1xNihLRUtGZV1xNyhLRktHZV1xOChLRktIZV1xOShLO0s8ZV1xOihLSktLZV1xOyhLSktTZV1xPChLS0tMZV1xPShLTEtNZV1xPihLTEtPZV1xPyhLTUtOZV1xQChLT0tQZV1xQShLT0tRZV1xQihLUUtSZV1xQyhLUUtTZV1xRChLU0tUZV1xRShLVEtVZV1xRihLVUtWZV1xRyhLVUtXZV1xSChLSUtKZV1xSShLO0s2ZV1xSihLWEtZZV1xSyhLWEtcZV1xTChLWUtaZV1xTShLWktbZV1xTihLXktfZV1xTyhLXktmZV1xUChLX0tgZV1xUShLYEthZV1xUihLYEtjZV1xUyhLYUtiZV1xVChLY0tkZV1xVShLY0trZV1xVihLZEtlZV1xVyhLZEtmZV1xWChLZktnZV1xWShLZ0toZV1xWihLaEtpZV1xWyhLaEtqZV1xXChLXUteZV1xXShLbEttZV1xXihLbEt1ZV1xXyhLbUtuZV1xYChLbktvZV1xYShLbktxZV1xYihLb0twZV1xYyhLcUtyZV1xZChLcUtzZV1xZShLc0t0ZV1xZihLc0t1ZV1xZyhLdUt2ZV1xaChLdkt3ZV1xaShLd0t4ZV1xaihLd0t5ZV1xayhLa0tsZV1xbChLWEtdZV1xbShLekt7ZV1xbihLekt+ZV1xbyhLe0t8ZV1xcChLfEt9ZV1xcShLgEuBZV1xcihLgEuIZV1xcyhLgUuCZV1xdChLgkuDZV1xdShLgkuFZV1xdihLg0uEZV1xdyhLhUuGZV1xeChLhUuNZV1xeShLhkuHZV1xeihLhkuIZV1xeyhLiEuJZV1xfChLiUuKZV1xfShLikuLZV1xfihLikuMZV1xfyhLf0uAZV1xgChLjkuPZV1xgShLjkuXZV1xgihLj0uQZV1xgyhLkEuRZV1xhChLkEuTZV1xhShLkUuSZV1xhihLk0uUZV1xhyhLk0uVZV1xiChLlUuWZV1xiShLlUuXZV1xiihLl0uYZV1xiyhLmEuZZV1xjChLmUuaZV1xjShLmUubZV1xjihLjUuOZV1xjyhLf0t6ZWVVBWxhYmVscZBLjFgAAAAAfYdVBnJhZGl1c3GRS4xHP8mZmaAAAAB9h1ULbGFiZWxPZmZzZXRxkkuMTn2HVQhkcmF3TW9kZXGTS4xLAX2HVQhvcHRpb25hbHGUfVUHZGlzcGxheXGVS4xLAn2HdS4='))
	crdInfo = cPickle.loads(base64.b64decode('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'))
	surfInfo = {'category': (0, None, {}), 'probeRadius': (0, None, {}), 'pointSize': (0, None, {}), 'name': [], 'density': (0, None, {}), 'colorMode': (0, None, {}), 'useLighting': (0, None, {}), 'transparencyBlendMode': (0, None, {}), 'molecule': [], 'smoothLines': (0, None, {}), 'lineWidth': (0, None, {}), 'allComponents': (0, None, {}), 'twoSidedLighting': (0, None, {}), 'customVisibility': [], 'drawMode': (0, None, {}), 'display': (0, None, {}), 'customColors': []}
	vrmlInfo = {'subid': (0, None, {}), 'display': (0, None, {}), 'id': (0, None, {}), 'vrmlString': [], 'name': (0, None, {})}
	colors = {'Ru': ((0.141176, 0.560784, 0.560784), 1, u'default'), 'Re': ((0.14902, 0.490196, 0.670588), 1, u'default'), 'Rf': ((0.8, 0, 0.34902), 1, u'default'), 'Ra': ((0, 0.490196, 0), 1, u'default'), 'Rb': ((0.439216, 0.180392, 0.690196), 1, u'default'), 'Rn': ((0.258824, 0.509804, 0.588235), 1, u'default'), 'Rh': ((0.0392157, 0.490196, 0.54902), 1, u'default'), 'Be': ((0.760784, 1, 0), 1, u'default'), 'Ba': ((0, 0.788235, 0), 1, u'default'), 'Bh': ((0.878431, 0, 0.219608), 1, u'default'), 'Bi': ((0.619608, 0.309804, 0.709804), 1, u'default'), 'Bk': ((0.541176, 0.309804, 0.890196), 1, u'default'), 'Br': ((0.65098, 0.160784, 0.160784), 1, u'default'), 'orange': ((1, 0.498039, 0), 1, u'default'), 'turquoise': ((0.25098, 0.878431, 0.815686), 1, u'default'), 'H': ((1, 1, 1), 1, u'default'), 'P': ((1, 0.501961, 0), 1, u'default'), 'Os': ((0.14902, 0.4, 0.588235), 1, u'default'), 'Ge': ((0.4, 0.560784, 0.560784), 1, u'default'), 'Gd': ((0.270588, 1, 0.780392), 1, u'default'), 'Ga': ((0.760784, 0.560784, 0.560784), 1, u'default'), 'Pr': ((0.85098, 1, 0.780392), 1, u'default'),
'Pt': ((0.815686, 0.815686, 0.878431), 1, u'default'), 'Pu': ((0, 0.419608, 1), 1, u'default'), 'C': ((0.564706, 0.564706, 0.564706), 1, u'default'), 'Pb': ((0.341176, 0.34902, 0.380392), 1, u'default'), 'Pa': ((0, 0.631373, 1), 1, u'default'), 'Pd': ((0, 0.411765, 0.521569), 1, u'default'), 'Cd': ((1, 0.85098, 0.560784), 1, u'default'), 'Po': ((0.670588, 0.360784, 0), 1, u'default'), 'Pm': ((0.639216, 1, 0.780392), 1, u'default'), 'steel blue': ((0.27451, 0.509804, 0.705882), 1, u'default'), 'Hs': ((0.901961, 0, 0.180392), 1, u'default'), 'Ho': ((0, 1, 0.611765), 1, u'default'), 'Hf': ((0.301961, 0.760784, 1), 1, u'default'), 'Hg': ((0.721569, 0.721569, 0.815686), 1, u'default'), 'He': ((0.85098, 1, 1), 1, u'default'), 'Md': ((0.701961, 0.0509804, 0.65098), 1, u'default'), 'Mg': ((0.541176, 1, 0), 1, u'default'), 'K': ((0.560784, 0.25098, 0.831373), 1, u'default'), 'Mn': ((0.611765, 0.478431, 0.780392), 1, u'default'), 'O': ((1, 0.0509804, 0.0509804), 1, u'default'), 'Mt': ((0.921569, 0, 0.14902), 1, u'default'), 'S': ((1, 1, 0.188235), 1, u'default'),
'W': ((0.129412, 0.580392, 0.839216), 1, u'default'), 'sky blue': ((0.529412, 0.807843, 0.921569), 1, u'default'), 'Zn': ((0.490196, 0.501961, 0.690196), 1, u'default'), 'plum': ((0.866667, 0.627451, 0.866667), 1, u'default'), 'Eu': ((0.380392, 1, 0.780392), 1, u'default'), 'Zr': ((0.580392, 0.878431, 0.878431), 1, u'default'), 'Er': ((0, 0.901961, 0.458824), 1, u'default'), 'Ni': ((0.313725, 0.815686, 0.313725), 1, u'default'), 'No': ((0.741176, 0.0509804, 0.529412), 1, u'default'), 'Na': ((0.670588, 0.360784, 0.94902), 1, u'default'), 'Nb': ((0.45098, 0.760784, 0.788235), 1, u'default'), 'Nd': ((0.780392, 1, 0.780392), 1, u'default'), 'Ne': ((0.701961, 0.890196, 0.960784), 1, u'default'), 'Np': ((0, 0.501961, 1), 1, u'default'), 'Fr': ((0.258824, 0, 0.4), 1, u'default'), 'Fe': ((0.878431, 0.4, 0.2), 1, u'default'), 'Fm': ((0.701961, 0.121569, 0.729412), 1, u'default'), 'B': ((1, 0.709804, 0.709804), 1, u'default'), 'F': ((0.564706, 0.878431, 0.313725), 1, u'default'), 'Sr': ((0, 1, 0), 1, u'default'), 'N': ((0.188235, 0.313725, 0.972549), 1, u'default'),
'Kr': ((0.360784, 0.721569, 0.819608), 1, u'default'), 'Si': ((0.941176, 0.784314, 0.627451), 1, u'default'), 'Sn': ((0.4, 0.501961, 0.501961), 1, u'default'), 'Sm': ((0.560784, 1, 0.780392), 1, u'default'), 'V': ((0.65098, 0.65098, 0.670588), 1, u'default'), 'Sc': ((0.901961, 0.901961, 0.901961), 1, u'default'), 'Sb': ((0.619608, 0.388235, 0.709804), 1, u'default'), 'Sg': ((0.85098, 0, 0.270588), 1, u'default'), 'Se': ((1, 0.631373, 0), 1, u'default'), 'Co': ((0.941176, 0.564706, 0.627451), 1, u'default'), 'Cm': ((0.470588, 0.360784, 0.890196), 1, u'default'), 'Cl': ((0.121569, 0.941176, 0.121569), 1, u'default'), 'Ca': ((0.239216, 1, 0), 1, u'default'), 'medium blue': ((0.196078, 0.196078, 0.803922), 1, u'default'), 'Cf': ((0.631373, 0.211765, 0.831373), 1, u'default'), 'Ce': ((1, 1, 0.780392), 1, u'default'), 'Xe': ((0.258824, 0.619608, 0.690196), 1, u'default'), 'Tm': ((0, 0.831373, 0.321569), 1, u'default'), 'light green': ((0.564706, 0.933333, 0.564706), 1, u'default'), 'Cs': ((0.341176, 0.0901961, 0.560784), 1, u'default'), 'Cr': ((0.541176, 0.6, 0.780392), 1, u'default'),
'Cu': ((0.784314, 0.501961, 0.2), 1, u'default'), 'La': ((0.439216, 0.831373, 1), 1, u'default'), 'Li': ((0.8, 0.501961, 1), 1, u'default'), 'Tl': ((0.65098, 0.329412, 0.301961), 1, u'default'), 'Lu': ((0, 0.670588, 0.141176), 1, u'default'), 'Lr': ((0.780392, 0, 0.4), 1, u'default'), 'Th': ((0, 0.729412, 1), 1, u'default'), 'Ti': ((0.74902, 0.760784, 0.780392), 1, u'default'), 'tan': ((0.823529, 0.705882, 0.54902), 1, u'default'), 'Te': ((0.831373, 0.478431, 0), 1, u'default'), 'Tb': ((0.188235, 1, 0.780392), 1, u'default'), 'Tc': ((0.231373, 0.619608, 0.619608), 1, u'default'), 'Ta': ((0.301961, 0.65098, 1), 1, u'default'), 'Yb': ((0, 0.74902, 0.219608), 1, u'default'), 'Db': ((0.819608, 0, 0.309804), 1, u'default'), 'Dy': ((0.121569, 1, 0.780392), 1, u'default'), 'At': ((0.458824, 0.309804, 0.270588), 1, u'default'), 'I': ((0.580392, 0, 0.580392), 1, u'default'), 'U': ((0, 0.560784, 1), 1, u'default'), 'Y': ((0.580392, 1, 1), 1, u'default'), 'Ac': ((0.439216, 0.670588, 0.980392), 1, u'default'), 'Ag': ((0.752941, 0.752941, 0.752941), 1, u'default'),
'Ir': ((0.0901961, 0.329412, 0.529412), 1, u'default'), 'Am': ((0.329412, 0.360784, 0.94902), 1, u'default'), 'Al': ((0.74902, 0.65098, 0.65098), 1, u'default'), 'As': ((0.741176, 0.501961, 0.890196), 1, u'default'), 'Ar': ((0.501961, 0.819608, 0.890196), 1, u'default'), 'Au': ((1, 0.819608, 0.137255), 1, u'default'), 'Es': ((0.701961, 0.121569, 0.831373), 1, u'default'), 'In': ((0.65098, 0.458824, 0.45098), 1, u'default'), 'Mo': ((0.329412, 0.709804, 0.709804), 1, u'default')}
	materials = {u'default': ((0.85, 0.85, 0.85), 30)}
	pbInfo = {'category': [u'distance monitor'], 'bondInfo': [{'color': (0, None, {}), 'atoms': [], 'label': (0, None, {}), 'halfbond': (0, None, {}), 'labelColor': (0, None, {}), 'drawMode': (0, None, {}), 'display': (0, None, {})}], 'lineType': (1, 2, {}), 'color': (1, 7, {}), 'optional': {'fixedLabels': (True, False, (1, False, {}))}, 'display': (1, True, {}), 'showStubBonds': (1, False, {}), 'lineWidth': (1, 1, {}), 'stickScale': (1, 1, {}), 'id': [-2]}
	modelAssociations = {}
	colorInfo = (10, (u'green', (0, 1, 0, 1)), {(u'steel blue', (0.27451, 0.509804, 0.705882, 1)): [5], (u'tan', (0.823529, 0.705882, 0.54902, 1)): [0], (u'plum', (0.866667, 0.627451, 0.866667, 1)): [2], (u'light green', (0.564706, 0.933333, 0.564706, 1)): [3], (u'turquoise', (0.25098, 0.878431, 0.815686, 1)): [6], (u'sky blue', (0.529412, 0.807843, 0.921569, 1)): [1], (u'white', (1, 1, 1, 1)): [8], (u'blue', (0, 0, 1, 1)): [4], (u'yellow', (1, 1, 0, 1)): [7]})
	viewerInfo = {'cameraAttrs': {'center': (2.25, 2.2785, -3.1295), 'fieldOfView': 16.1331, 'nearFar': (12.1746, -18.1724), 'ortho': False, 'eyeSeparation': 50.8, 'focal': -3.1295}, 'viewerAttrs': {'silhouetteColor': None, 'clipping': False, 'showSilhouette': False, 'showShadows': False, 'viewSize': 10.542, 'labelsOnTop': True, 'depthCueRange': (0.5, 1), 'silhouetteWidth': 2, 'singleLayerTransparency': True, 'shadowTextureSize': 2048, 'backgroundImage': [None, 1, 2, 1, 0, 0], 'backgroundGradient': [('Chimera default', [(1, 1, 1, 1), (0, 0, 1, 1)], 1), 1, 0, 0], 'depthCue': True, 'highlight': 0, 'scaleFactor': 1.03326, 'angleDependentTransparency': True, 'backgroundMethod': 0}, 'viewerHL': 9, 'cameraMode': 'mono', 'detail': 1.5, 'viewerFog': None, 'viewerBG': 8}

	replyobj.status("Initializing session restore...", blankAfter=0,
		secondary=True)
	from SimpleSession.versions.v58 import expandSummary
	init(dict(enumerate(expandSummary(colorInfo))))
	replyobj.status("Restoring colors...", blankAfter=0,
		secondary=True)
	restoreColors(colors, materials)
	replyobj.status("Restoring molecules...", blankAfter=0,
		secondary=True)
	restoreMolecules(molInfo, resInfo, atomInfo, bondInfo, crdInfo)
	replyobj.status("Restoring surfaces...", blankAfter=0,
		secondary=True)
	restoreSurfaces(surfInfo)
	replyobj.status("Restoring VRML models...", blankAfter=0,
		secondary=True)
	restoreVRML(vrmlInfo)
	replyobj.status("Restoring pseudobond groups...", blankAfter=0,
		secondary=True)
	restorePseudoBondGroups(pbInfo)
	replyobj.status("Restoring model associations...", blankAfter=0,
		secondary=True)
	restoreModelAssociations(modelAssociations)
	replyobj.status("Restoring camera...", blankAfter=0,
		secondary=True)
	restoreViewer(viewerInfo)

try:
	restoreCoreModels()
except:
	reportRestoreError("Error restoring core models")

	replyobj.status("Restoring extension info...", blankAfter=0,
		secondary=True)


try:
	import StructMeasure
	from StructMeasure.DistMonitor import restoreDistances
	registerAfterModelsCB(restoreDistances, 1)
except:
	reportRestoreError("Error restoring distances in session")


def restoreMidasBase():
	formattedPositions = {}
	import Midas
	Midas.restoreMidasBase(formattedPositions)
try:
	restoreMidasBase()
except:
	reportRestoreError('Error restoring Midas base state')


def restoreMidasText():
	from Midas import midas_text
	midas_text.aliases = {}
	midas_text.userSurfCategories = {}

try:
	restoreMidasText()
except:
	reportRestoreError('Error restoring Midas text state')

geomData = {'AxisManager': {}, 'CentroidManager': {}, 'PlaneManager': {}}

try:
	from StructMeasure.Geometry import geomManager
	geomManager._restoreSession(geomData)
except:
	reportRestoreError("Error restoring geometry objects in session")


def restoreSession_RibbonStyleEditor():
	import SimpleSession
	import RibbonStyleEditor
	userScalings = [('licorice', [[0.35, 0.35], [0.35, 0.35], [0.35, 0.35], [0.35, 0.35, 0.35, 0.35], [0.35, 0.35]])]
	userXSections = []
	userResidueClasses = []
	residueData = [(4, 'Chimera default', 'rounded', u'amino acid'), (5, 'Chimera default', 'rounded', u'unknown'), (6, 'Chimera default', 'rounded', u'unknown'), (7, 'Chimera default', 'rounded', u'unknown'), (8, 'Chimera default', 'rounded', u'amino acid'), (9, 'Chimera default', 'rounded', u'unknown'), (10, 'Chimera default', 'rounded', u'unknown'), (11, 'Chimera default', 'rounded', u'unknown'), (12, 'Chimera default', 'rounded', u'amino acid'), (13, 'Chimera default', 'rounded', u'unknown'), (14, 'Chimera default', 'rounded', u'unknown'), (15, 'Chimera default', 'rounded', u'unknown'), (16, 'Chimera default', 'rounded', u'amino acid'), (17, 'Chimera default', 'rounded', u'unknown'), (18, 'Chimera default', 'rounded', u'unknown'), (19, 'Chimera default', 'rounded', u'unknown')]
	flags = RibbonStyleEditor.NucleicDefault1
	SimpleSession.registerAfterModelsCB(RibbonStyleEditor.restoreState,
				(userScalings, userXSections,
				userResidueClasses, residueData, flags))
try:
	restoreSession_RibbonStyleEditor()
except:
	reportRestoreError("Error restoring RibbonStyleEditor state")

trPickle = 'gAJjQW5pbWF0ZS5UcmFuc2l0aW9ucwpUcmFuc2l0aW9ucwpxASmBcQJ9cQMoVQxjdXN0b21fc2NlbmVxBGNBbmltYXRlLlRyYW5zaXRpb24KVHJhbnNpdGlvbgpxBSmBcQZ9cQcoVQZmcmFtZXNxCEsBVQ1kaXNjcmV0ZUZyYW1lcQlLAVUKcHJvcGVydGllc3EKXXELVQNhbGxxDGFVBG5hbWVxDWgEVQRtb2RlcQ5VBmxpbmVhcnEPdWJVCGtleWZyYW1lcRBoBSmBcRF9cRIoaAhLFGgJSwFoCl1xE2gMYWgNaBBoDmgPdWJVBXNjZW5lcRRoBSmBcRV9cRYoaAhLAWgJSwFoCl1xF2gMYWgNaBRoDmgPdWJ1Yi4='
scPickle = 'gAJjQW5pbWF0ZS5TY2VuZXMKU2NlbmVzCnEBKYFxAn1xA1UHbWFwX2lkc3EEfXNiLg=='
kfPickle = 'gAJjQW5pbWF0ZS5LZXlmcmFtZXMKS2V5ZnJhbWVzCnEBKYFxAn1xA1UHZW50cmllc3EEXXEFc2Iu'
def restoreAnimation():
	'A method to unpickle and restore animation objects'
	# Scenes must be unpickled after restoring transitions, because each
	# scene links to a 'scene' transition. Likewise, keyframes must be 
	# unpickled after restoring scenes, because each keyframe links to a scene.
	# The unpickle process is left to the restore* functions, it's 
	# important that it doesn't happen prior to calling those functions.
	import SimpleSession
	from Animate.Session import restoreTransitions
	from Animate.Session import restoreScenes
	from Animate.Session import restoreKeyframes
	SimpleSession.registerAfterModelsCB(restoreTransitions, trPickle)
	SimpleSession.registerAfterModelsCB(restoreScenes, scPickle)
	SimpleSession.registerAfterModelsCB(restoreKeyframes, kfPickle)
try:
	restoreAnimation()
except:
	reportRestoreError('Error in Animate.Session')

def restoreLightController():
	import Lighting
	Lighting._setFromParams({'ratio': 1.25, 'brightness': 1.16, 'material': [30.0, (0.85, 0.85, 0.85), 1.0], 'back': [(0.35740674433659325, 0.6604015517481454, -0.6604015517481455), (1.0, 1.0, 1.0), 0.0], 'mode': 'two-point', 'key': [(-0.35740674433659325, 0.6604015517481454, 0.6604015517481455), (1.0, 1.0, 1.0), 1.0], 'contrast': 0.83, 'fill': [(0.25056280708573153, 0.25056280708573153, 0.9351131265310293), (1.0, 1.0, 1.0), 0.0]})
try:
	restoreLightController()
except:
	reportRestoreError("Error restoring lighting parameters")


try:
	from BuildStructure.gui import _sessionRestore
	_sessionRestore({'mapped': 1})
except:
	reportRestoreError("Failure restoring Build Structure")


def restoreRemainder():
	from SimpleSession.versions.v58 import restoreWindowSize, \
	     restoreOpenStates, restoreSelections, restoreFontInfo, \
	     restoreOpenModelsAttrs, restoreModelClip

	curSelIds =  []
	savedSels = []
	openModelsAttrs = { 'cofrMethod': 4 }
	windowSize = (845, 898)
	xformMap = {0: (((-0.153754, 0.840351, 0.519779), 62.2921), (7.03964, 0.471088, -3.38657), True), 1: (((0.155307, 0.85084, 0.501947), 146.664), (3.96678, 1.29022, -4.16952), True), 2: (((-0.543806, -0.812992, -0.208134), 84.3501), (3.43969, 0.856731, -0.98636), True), 3: (((0.0118848, -0.958404, 0.285169), 158.397), (0.433923, -3.23352, -5.73395), True)}
	fontInfo = {'face': ('Sans Serif', 'Normal', 16)}
	clipPlaneInfo = {}

	replyobj.status("Restoring window...", blankAfter=0,
		secondary=True)
	restoreWindowSize(windowSize)
	replyobj.status("Restoring open states...", blankAfter=0,
		secondary=True)
	restoreOpenStates(xformMap)
	replyobj.status("Restoring font info...", blankAfter=0,
		secondary=True)
	restoreFontInfo(fontInfo)
	replyobj.status("Restoring selections...", blankAfter=0,
		secondary=True)
	restoreSelections(curSelIds, savedSels)
	replyobj.status("Restoring openModel attributes...", blankAfter=0,
		secondary=True)
	restoreOpenModelsAttrs(openModelsAttrs)
	replyobj.status("Restoring model clipping...", blankAfter=0,
		secondary=True)
	restoreModelClip(clipPlaneInfo)

	replyobj.status("Restoring remaining extension info...", blankAfter=0,
		secondary=True)
try:
	restoreRemainder()
except:
	reportRestoreError("Error restoring post-model state")
from SimpleSession.versions.v58 import makeAfterModelsCBs
makeAfterModelsCBs()

from SimpleSession.versions.v58 import endRestore
replyobj.status('Finishing restore...', blankAfter=0, secondary=True)
endRestore({})
replyobj.status('', secondary=True)
replyobj.status('Restore finished.')

