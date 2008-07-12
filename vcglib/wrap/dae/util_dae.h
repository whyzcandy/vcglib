/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004-2008                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *   
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/
#ifndef __VCGLIB_UTILDAE
#define __VCGLIB_UTILDAE


#include <wrap/io_trimesh/additionalinfo.h>
#include <vcg/complex/trimesh/update/normal.h>
#include <vcg/complex/trimesh/allocate.h>

#include <wrap/io_trimesh/io_mask.h>

#include<QtXml/QDomDocument>
#include<QtCore/QFile>
#include <QtXml/QXmlStreamWriter>
#include <QtCore/QStringList>

#include<vcg/space/point3.h>
#include<vcg/space/texcoord2.h>
#include<vcg/space/color4.h>
#include<vcg/space/texcoord2.h>
#include <wrap/callback.h>

#include <vector>

#include <vcg/simplex/edge/edge.h>

#include <wrap/gl/glu_tesselator.h>

namespace vcg {
namespace tri {
namespace io {
	class InfoDAE
	{
		public:

		InfoDAE()
		{
			doc = NULL;
		}

		~InfoDAE()
		{
			delete doc;
		}

		QDomDocument* doc;		
	};

	class AdditionalInfoDAE : public AdditionalInfo
	{
	public: 
		vcg::tri::io::InfoDAE* dae;
		std::vector<QString> texturefile; 

		AdditionalInfoDAE()
		:AdditionalInfo()
		{
			mask	= 0;
			numvert = 0;
			numface = 0;
		}

		~AdditionalInfoDAE()
		{
			delete dae;
			texturefile.clear();
		}
	};

	class UtilDAE

	{
	public:
		enum DAEError 
		{
			E_NOERROR,				// 0
			E_CANTOPEN,				// 1
			E_NOGEOMETRYLIBRARY,     // 2 
			E_NOMESH,      // 3
			E_NOVERTEXPOSITION,            // 4
			E_NO3DVERTEXPOSITION,			// 5
			E_NO3DSCENE, // 6
			E_INCOMPATIBLECOLLADA141FORMAT, //7
			E_UNREFERENCEBLEDCOLLADAATTRIBUTE, // 8
			E_NOPOLYGONALMESH, //9
			E_CANTSAVE //10
		};

		

		template<typename VERTEX_TYPE>
		class MyPolygon 
		{
		public:
			typedef VERTEX_TYPE BaseVertexType;

			int _nvert;
			std::vector<VERTEX_TYPE*> _pv;
			std::vector< vcg::TexCoord2<float> > _txc;


			MyPolygon(int n)
			:_nvert(n),_pv(_nvert),_txc(_nvert)
			{
			}
		};

		template<typename POLYGONAL_TYPE>
		class PolygonalMesh
		{
		public:
			typedef POLYGONAL_TYPE FaceType;

			enum PERWEDGEATTRIBUTETYPE {NONE = 0,NORMAL = 1,MULTITEXTURECOORD = 2,MULTICOLOR = 4};

			typedef typename FaceType::BaseVertexType VertexType;
			typedef VertexType* VertexPointer;
			typedef typename std::vector<VertexType>::iterator VertexIterator; 
			typedef typename std::vector<FaceType>::iterator PolygonIterator; 

			vcg::Box3<float> bbox;

			std::vector<VertexType> vert;
			std::vector<FaceType> _pols;

			void generatePointsVector(std::vector<std::vector<vcg::Point3f> >& v)
			{
				for(typename PolygonalMesh::PolygonIterator itp = _pols.begin();itp != _pols.end();++itp)
				{
					v.push_back(std::vector<vcg::Point3f>());
					for(typename std::vector<VertexPointer>::iterator itv = itp->_pv.begin();itv != itp->_pv.end();++itv)
					{
						v[v.size() - 1].push_back((*itv)->P());
					}	
				}
			}

			void usePerWedgeAttributes(PERWEDGEATTRIBUTETYPE att,const unsigned int multitexture = 1,const unsigned int multicolor = 1)
			{
				if (att != NONE)
				{
					for(PolygonIterator itp = _pols.begin();itp != _pols.end();++itp)
					{
						if (att & MULTICOLOR) itp->usePerWedgeColor(multicolor);
						if (att & MULTITEXTURECOORD) itp->usePerWedgeMultiTexture(multitexture);
						if (att & NORMAL) itp->usePerWedgeNormal();
					}
				}
			}

			template<class TRIMESH>
			void triangulate(TRIMESH& mesh)
			{
				std::vector<std::vector<vcg::Point3f> > pl;
				mesh.vert.resize(vert.size());
				int multicoor = 0;
				//PolygonalMesh's points has been copied in TriangularMesh
				for(size_t jj = 0;jj < mesh.vert.size();++jj)
					mesh.vert[jj].P() = vert[jj].P();

				bool texen = mesh.face.IsWedgeTexEnabled();
				unsigned int totaltri = 0;
				for(size_t ii = 0;ii < _pols.size();++ii)
						totaltri += _pols[ii]._nvert - 2;
				
				mesh.face.resize(totaltri);

				//transform the polygonal mesh in a vector<vector<Point>>
				generatePointsVector(pl);


				int trioff = 0;
				//foreach Polygon
				for(size_t ii = 0;ii < pl.size();++ii)
				{
					std::vector<int> tx;
					std::vector<std::vector<vcg::Point3f> > pl2(1);
					pl2[0] = pl[ii];

					vcg::glu_tesselator::tesselate(pl2,tx);
					size_t ntri = tx.size() / 3;
					assert(tx.size() % 3 == 0);
					

					int polvert = 0;
					//foreach triangle
					for(size_t tr = 0;tr < ntri;++tr)
					{
						
						//typename TRIMESH::FaceType& f = mesh.face[tr];

						//typename TRIMESH::FaceType& f = mesh.face[tr];
						for(unsigned int tt = 0;tt < 3; ++tt)
						{
							mesh.face[trioff + tr].V(tt) = &(mesh.vert[_pols[ii]._pv[tx[3 * tr + tt]] - &(vert[0])]);
							//vcg::Point3f ppp = mesh.face[tr].V(tt)->P();
							if (texen)
							{
							/*	f.WT(multicoor).U() = _pols[ii]._txc[polvert].U();
								f.WT(multicoor).V() = _pols[ii]._txc[polvert].V();
								f.WT(multicoor).N() = _pols[ii]._txc[polvert].N();*/
								
							}
							polvert = (polvert + 1) % _pols[ii]._nvert;
						}
						//mesh.face.push_back(f);
					}
					trioff += ntri;
				}
				assert(trioff == totaltri);
			}
		};

		static const char *ErrorMsg(int error)
		{
			static const char * dae_error_msg[] =
			{
				"No errors",
				"Can't open file",
				"File without a geometry library",
				"There isn't mesh in file",
				"The meshes in file haven't the vertex position attribute",
				"The importer assumes that the OpenMeshType uses a 3D point for the vertex position",
				"There isn't any scene in Collada file",
				"The input file is not compatible with COLLADA 1.41 standard format",
				"Collada file is trying to referece an attribute that is not in the file",
				"This version of Collada Importer support only triangular and polygonal mesh file"
			};

			if(error>9 || error<0) return "Unknown error";
			else return dae_error_msg[error];
		};
	protected:
		inline static void referenceToANodeAttribute(const QDomNode n,const QString& attr,QString& url_st)
		{
			url_st = n.toElement().attribute(attr);
			int sz = url_st.size() - 1;
			url_st = url_st.right(sz);
			assert(url_st.size() != 0);
		}

		inline static QDomNode findNodeBySpecificAttributeValue(const QDomNodeList& ndl,const QString& attrname,const QString& attrvalue)
		{
			int ndl_size = ndl.size();
			int ind = 0;
			while(ind < ndl_size)
			{
				QString st = ndl.at(ind).toElement().attribute(attrname);
				if (st == attrvalue)
					return ndl.at(ind);
				++ind;
			}
			return QDomNode();
		}

		inline static QDomNode findNodeBySpecificAttributeValue(const QDomNode n,const QString& tag,const QString& attrname,const QString& attrvalue)
		{
			return findNodeBySpecificAttributeValue(n.toElement().elementsByTagName(tag),attrname,attrvalue);
		}

		inline static QDomNode findNodeBySpecificAttributeValue(const QDomDocument n,const QString& tag,const QString& attrname,const QString& attrvalue)
		{
			return findNodeBySpecificAttributeValue(n.elementsByTagName(tag),attrname,attrvalue);
		}
		
		inline static bool isThereTag(const QDomNodeList& list)
		{
			return ((list.size() > 0) ? true : false);
		}

		inline static bool isThereTag(const QDomNode n,const QString& tagname)
		{
			return isThereTag(n.toElement().elementsByTagName(tagname));
		}

		inline static bool isThereTag(const QDomDocument n,const QString& tagname)
		{
			return isThereTag(n.elementsByTagName(tagname));
		}


		inline static QDomNode attributeSourcePerSimplex(const QDomNode n,const QDomDocument startpoint,const QString& sem)
		{
			QDomNodeList vertattr = n.toElement().elementsByTagName("input");
			for(int ind = 0;ind < vertattr.size();++ind)
			{
				if (vertattr.at(ind).toElement().attribute("semantic") == sem)
				{
					QString url; 
					referenceToANodeAttribute(vertattr.at(ind),"source",url);
					return findNodeBySpecificAttributeValue(startpoint,"source","id",url);
				}
			}
			return QDomNode();
		}

		inline static void valueStringList(QStringList& res,const QDomNode srcnode,const QString& tag) 
		{
			QDomNodeList list = srcnode.toElement().elementsByTagName(tag);
			assert(list.size() == 1);
			QString nd = list.at(0).firstChild().nodeValue();
			res = nd.split(" ");
			if (res.last() == "")
				res.removeLast();
		
		}

		/*inline static bool removeChildNode(QDomNodeList*/
		
		inline static bool removeChildNodeList(QDomNodeList& nodelst,const QString& tag = "", const QString& attribname = "", const QString& attribvalue = "")
		{
			for(int jj = 0;jj < nodelst.size();++jj)
			{
				removeChildNode(nodelst.at(jj),tag,attribname,attribvalue); 
			}
			return true;
		}


		inline static bool removeChildNode(QDomNode node,const QString& tag = "", const QString& attribname = "", const QString& attribvalue = "")
		{
			QDomNodeList clst = node.childNodes();
			for(int ii = 0;ii < clst.size();++ii)
			{
				QDomNode oldchild = node.childNodes().at(ii); 
				if (tag != "")
				{
					if ((attribname != "") && (attribvalue != ""))
					{
						if (clst.at(ii).toElement().attribute(attribname) == attribvalue)
							node.removeChild(oldchild);
					}
					else 
					{	
						QString nm = clst.at(ii).nodeName();
						if (clst.at(ii).nodeName() == tag) 
						{
							node.removeChild(oldchild);
						}
					}
				}
				else node.removeChild(oldchild);
			}
			return true;
		}

		static void ParseRotationMatrix(vcg::Matrix44f& m,const std::vector<QDomNode>& t)
		{
			vcg::Matrix44f tmp;
			tmp.SetIdentity();
			for(unsigned int ii = 0;ii < t.size();++ii)
			{
				QString rt = t[ii].firstChild().nodeValue();
				QStringList rtl = rt.split(" ");
				if (rtl.last() == "") rtl.removeLast();
				assert(rtl.size() == 4);
				tmp.SetRotate(rtl.at(3).toFloat(),vcg::Point3f(rtl.at(0).toFloat(),rtl.at(1).toFloat(),rtl.at(2).toFloat()));
				tmp *= tmp;
			}
			m = m * tmp;
		}

		static void AddTranslation(vcg::Matrix44f& m,const QDomNode t)
		{
			QDomNode tr = t.firstChild();
			QString coord = tr.nodeValue();
			QStringList coordlist = coord.split(" ");
			if (coordlist.last() == "") 
				coordlist.removeLast();
			assert(coordlist.size() == 3);
			m[0][0] = 1.0f;
			m[1][1] = 1.0f;
			m[2][2] = 1.0f;
			m[3][3] = 1.0f;
			m[0][3] = coordlist.at(0).toFloat();
			m[1][3] = coordlist.at(1).toFloat();
			m[2][3] = coordlist.at(2).toFloat();
		}

		static void TransfMatrix(const QDomNode parentnode,const QDomNode presentnode,vcg::Matrix44f& m)
		{
			if (presentnode == parentnode) return;
			else
			{
				QDomNode par = presentnode.parentNode();
				std::vector<QDomNode> rotlist;
				QDomNode trans;
				for(int ch = 0;ch < par.childNodes().size();++ch)
				{
					if (par.childNodes().at(ch).nodeName() == "rotate")
						rotlist.push_back(par.childNodes().at(ch));
					else if (par.childNodes().at(ch).nodeName() == "translate")
						 {
							trans = par.childNodes().at(ch);
					     }		
				}
				vcg::Matrix44f tmp;
				tmp.SetIdentity();
				if (!trans.isNull()) AddTranslation(tmp,trans);
				ParseRotationMatrix(tmp,rotlist);
				m = m * tmp;
				TransfMatrix(parentnode,par,m);
			}
		}

		inline static int findOffSetForASingleSimplex(QDomNode node)
		{
			QDomNodeList wedatts = node.toElement().elementsByTagName("input");
			int max = 0;
			if (wedatts.size() == 0) return -1;
			else 
			{
				for(int ii = 0;ii < wedatts.size();++ii)
				{
					int tmp = wedatts.at(ii).toElement().attribute("offset").toInt();
					if (tmp > max) max = tmp;
				}
			}
			return max + 1;
		}

		inline static int findStringListAttribute(QStringList& list,const QDomNode node,const QDomNode poly,const QDomDocument startpoint,const char* token)
		{
			int offset;
			if (!node.isNull())
			{
				offset = node.toElement().attribute("offset").toInt();
				QDomNode st = attributeSourcePerSimplex(poly,startpoint,token);
				valueStringList(list,st,"float_array");
			}
			return offset;
		}

		inline static QDomNode textureFinder(const QString& textname,const QDomDocument doc)
		{
			//visual_scene -> instance_material
			QDomNodeList vis_scn = doc.elementsByTagName("library_visual_scenes");
			if (vis_scn.size() != 1) 
				return QDomNode();
			QDomNode symb = findNodeBySpecificAttributeValue(vis_scn.at(0),QString("instance_material"),QString("symbol"),textname);
			if (symb.isNull()) 
				return QDomNode();
			QString tar = symb.toElement().attribute("target");
			if (tar.isNull())
				return QDomNode();
			tar = tar.remove('#');

			//library_material -> material -> instance_effect
			QDomNodeList lib_mat = doc.elementsByTagName("library_materials");
			if (lib_mat.size() != 1) 
				return QDomNode();
			QDomNode material = findNodeBySpecificAttributeValue(lib_mat.at(0),QString("material"),QString("id"),tar);
			if (material.isNull()) 
				return QDomNode();
			QDomNodeList in_eff = material.toElement().elementsByTagName("instance_effect");
			if (vis_scn.size() == 0) 
				return QDomNode();
			QString url = in_eff.at(0).toElement().attribute("url");
			if ((url.isNull()) || (url == ""))
				return QDomNode();
			url = url.remove('#');

			//library_effects -> effect -> instance_effect
			QDomNodeList lib_eff = doc.elementsByTagName("library_effects");
			if (lib_eff.size() != 1) 
				return QDomNode();
			QDomNode effect = findNodeBySpecificAttributeValue(lib_eff.at(0),QString("effect"),QString("id"),url);
			if (effect.isNull()) 
				return QDomNode();
			QDomNodeList init_from = effect.toElement().elementsByTagName("init_from");
			if (init_from.size() == 0)
				return QDomNode();
			QString img_id = init_from.at(0).toElement().text();
			if ((img_id.isNull()) || (img_id == ""))
				return QDomNode();
			
			//library_images -> image
			QDomNodeList lib_img = doc.elementsByTagName("library_images");
			if (lib_img.size() != 1) 
				return QDomNode();
			QDomNode img = findNodeBySpecificAttributeValue(lib_img.at(0),QString("image"),QString("id"),img_id);
			if (img.isNull()) 
				return QDomNode();
			return img;			
		}

		static int indexTextureByImgNode(const QDomDocument doc,const QDomNode node)
		{	
			QDomNodeList libim = doc.elementsByTagName(QString("library_images"));
			if (libim.size() != 1)
				return -1;
			QDomNodeList imgs = libim.at(0).toElement().elementsByTagName("image");
			
			int ii = 0;
			bool found = false;
			while((ii < imgs.size()) && (!found))
			{
				if (imgs.at(ii) == node) 
					found = true;
				else ++ii;
			}
			if (found) 
				return ii;
			else
				return -1;
		}

		struct WedgeAttribute
		{
			QDomNode wnsrc;
			QStringList wn;
			int offnm;

			QDomNode wtsrc;
			QStringList wt;
			int stride;
			int offtx;

			QDomNode wcsrc;
			QStringList wc;
			int offcl;
		};
	};
}
}
}

#endif
