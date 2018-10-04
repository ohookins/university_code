//SDL.cpp, fsh, 11/4/99 adjusted 1/30/2000 for ECE661 to admit simple raytracing
// support code for the classes in SDL.h
// some pieces not implemented - left for students to do
#include "SDL.h"

bool debug = false;
bool info = false;

//<<<<<<<<<<<<<<<<<< dot3D >>>>>>>>>>>>>>>>
float dot3D(Vector3& a, Vector3& b){return a.x * b.x + a.y * b.y + a.z * b.z;}
float dot3D(Point3&  a, Vector3& b){return a.x * b.x + a.y * b.y + a.z * b.z;}
float dot3D(Point3&  a, Point3& b) {return a.x * b.x + a.y * b.y + a.z * b.z;}
//<<<<<<<<<<<<<<<<<<<< rayPos >>>>>>>>>>>>>>>>>
Point3 rayPos(Point3& s, Vector3& v, float t)
{
  Point3 p(s.x + v.x * t, 
	   s.y + v.y * t, 
	   s.z + v.z * t);
  return p;
}
//<<<<<<<<<<<<<<<< rayPos >>>>>>>>>>>>>>>>>>>>>..
Point3 rayPos(Ray&r, float t)
{
  Point3 p(r.start.x + r.dir.x * t, 
	   r.start.y + r.dir.y * t, 
	   r.start.z + r.dir.z * t);
  return p;
}
//<<<<<<<<<<<<<<<<<<<< xfrmPoint >>>>>>>>>>>...
void xfrmPoint(Point3& res, Affine4& aff,Point3& pt)
{ // form res = aff * pt - include translation part
  res.x = aff.m[0] * pt.x + aff.m[4] * pt.y + aff.m[8]  * pt.z + aff.m[12];
  res.y = aff.m[1] * pt.x + aff.m[5] * pt.y + aff.m[9]  * pt.z + aff.m[13];
  res.z = aff.m[2] * pt.x + aff.m[6] * pt.y + aff.m[10] * pt.z + aff.m[14];
}
//<<<<<<<<<<<<<<<<<<<< xfrmVect >>>>>>>>>>>...
void xfrmVect(Vector3& res, Affine4& aff,Vector3& v)
{ // form res = aff * v 
  res.x = aff.m[0] * v.x + aff.m[4] * v.y + aff.m[8]  * v.z;
  res.y = aff.m[1] * v.x + aff.m[5] * v.y + aff.m[9]  * v.z;
  res.z = aff.m[2] * v.x + aff.m[6] * v.y + aff.m[10] * v.z;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<< xfrmRay >>>>>>>>>>>>>>>>>>>>>
void xfrmRay(Ray& g, Affine4& aff, Ray& r)
{// g = aff * r // only the start part gets the translation component.
  g.start.x = aff.m[0] * r.start.x + aff.m[4] * r.start.y + aff.m[8]  * r.start.z + aff.m[12];
  g.start.y = aff.m[1] * r.start.x + aff.m[5] * r.start.y + aff.m[9]  * r.start.z + aff.m[13];
  g.start.z = aff.m[2] * r.start.x + aff.m[6] * r.start.y + aff.m[10] * r.start.z + aff.m[14];
  g.dir.x = aff.m[0] * r.dir.x + aff.m[4] * r.dir.y + aff.m[8]  * r.dir.z;
  g.dir.y = aff.m[1] * r.dir.x + aff.m[5] * r.dir.y + aff.m[9]  * r.dir.z;
  g.dir.z = aff.m[2] * r.dir.x + aff.m[6] * r.dir.y + aff.m[10] * r.dir.z;
}
//<<<<<<<<<<<<<<<<<<<< xfrmNormal >>>>>>>>>>>...
void xfrmNormal(Vector3& res, Affine4& aff,Vector3& v)
{ // form res = transpose(aff) * v 
  res.x = aff.m[0] * v.x + aff.m[1] * v.y + aff.m[2]  * v.z;
  res.y = aff.m[4] * v.x + aff.m[5] * v.y + aff.m[6]  * v.z;
  res.z = aff.m[8] * v.x + aff.m[9] * v.y + aff.m[10] * v.z;
}
// Vector3 methods
Vector3 Vector3 :: cross(Vector3 b) //return this cross b
{
  Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
  return c;
}
float Vector3 :: dot(Vector3 b) // return this dotted with b
{return x * b.x + y * b.y + z * b.z;}
void Vector3 :: normalize()//adjust this vector to unit length
{		
  double sizeSq = x * x + y * y + z * z;
  if(sizeSq < 0.0000001)
    {
      cerr << "\nnormalize() sees vector (0,0,0)!";
      return; // does nothing to zero vectors;
    }
  float scaleFactor = 1.0/(float)sqrt(sizeSq);
  x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}
// Color3 methods
void Color3 ::add(Color3& src, Color3& refl)
{ // add the product of source color and reflection coefficient
  red   += src.red   * refl.red;
  green += src.green * refl.green; 
  blue  += src.blue  * refl.blue;
}
void Color3:: add(Color3& colr)
{ // add colr to this color
  red += colr.red ; green += colr.green; blue += colr.blue;}

void Color3 :: build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
  v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
}
//Affine4 methods
Affine4::Affine4(){ // make identity transform
  m[0] = m[5]  = m[10] = m[15] = 1.0;
  m[1] = m[2]  = m[3]  = m[4]  = 0.0;
  m[6] = m[7]  = m[8]  = m[9]  = 0.0;
  m[11]= m[12] = m[13] = m[14] = 0.0;
}
void Affine4 :: setIdentityMatrix(){ // make identity transform
  m[0] = m[5]  = m[10] = m[15] = 1.0;
  m[1] = m[2]  = m[3]  = m[4]  = 0.0;
  m[6] = m[7]  = m[8]  = m[9]  = 0.0;
  m[11]= m[12] = m[13] = m[14] = 0.0;
}
void Affine4 ::set(Affine4 a)// set this matrix to a
{
  for(int i = 0; i < 16; i++)
    m[i]=a.m[i];
}
//<<<<<<<<<<<<<< preMult >>>>>>>>>>>>
void Affine4 ::preMult(Affine4 n)
{// postmultiplies this with n
  float sum;
  Affine4 tmp; 
  tmp.set(*this); // tmp copy
  // following mult's : this = tmp * n
  for(int c = 0; c < 4; c++)
    for(int r = 0; r <4 ; r++)
      {
	sum = 0;
	for(int k = 0; k < 4; k++)
	  sum += n.m[4 * k + r]* tmp.m[4 * c + k];
	m[4 * c + r] = sum;
      }// end of for loops
}// end of preMult()	
//<<<<<<<<<<<< postMult >>>>>>>>>>>
void Affine4 ::postMult(Affine4 n){// postmultiplies this with n
  float sum;
  Affine4 tmp; 
  tmp.set(*this); // tmp copy
  for(int c = 0; c < 4; c++)// form this = tmp * n
    for(int r = 0; r <4 ; r++)
      {
	sum = 0;
	for(int k = 0; k < 4; k++)
	  sum += tmp.m[4 * k + r]* n.m[4 * c + k];
	m[4 * c + r] = sum;
      }// end of for loops
}
// AffineStack methods
void AffineStack :: dup()
{
  AffineNode* tmp = new AffineNode;			
  tmp->affn = new Affine4(*(tos->affn));
  tmp->invAffn = new Affine4(*(tos->invAffn));
  tmp->next = tos;
  tos = tmp;
}
void AffineStack :: setIdentity() // make top item the identity matrix
{ 
  assert(tos != NULL);
  tos->affn->setIdentityMatrix();
  tos->invAffn->setIdentityMatrix();
}
void AffineStack :: popAndDrop()
{
  if(tos == NULL) return; // do nothing
  AffineNode *tmp = tos;
  tos = tos->next;
  delete tmp; // should call destructor, which deletes trices
}	
void AffineStack :: releaseAffines()
{ // pop and drop all remaining items 
  while(tos) popAndDrop();
}
void AffineStack :: rotate(float angle, Vector3 u)
{
  Affine4 rm; // make identity matrix
  Affine4 invRm;
  u.normalize(); // make the rotation axis unit length
  float ang = angle * 3.14159265/ 180; // deg to  
  float c = cos(ang), s = sin(ang);
  float mc = 1.0 - c;
  //fill the 3x3 upper left matrix - Chap.5 p. 29
  rm.m[0] = c + mc * u.x * u.x;
  rm.m[1] = mc * u.x * u.y + s * u.z;
  rm.m[2] = mc * u.x * u.z - s * u.y;
  rm.m[4] = mc * u.y * u.x - s * u.z;
  rm.m[5] = c + mc * u.y * u.y;
  rm.m[6] = mc * u.y * u.z + s * u.x;
  rm.m[8] = mc * u.z * u.x + s * u.y;
  rm.m[9] = mc * u.z * u.y - s * u.x;
  rm.m[10] = c + mc * u.z * u.z;
  // same for inverse : just sign of s is changed
  invRm.m[0] = c + mc * u.x * u.x;
  invRm.m[1] = mc * u.x * u.y - s * u.z;
  invRm.m[2] = mc * u.x * u.z + s * u.y;
  invRm.m[4] = mc * u.y * u.x + s * u.z;
  invRm.m[5] = c + mc * u.y * u.y;
  invRm.m[6] = mc * u.y * u.z - s * u.x;
  invRm.m[8] = mc * u.z * u.x - s * u.y;
  invRm.m[9] = mc * u.z * u.y + s * u.x;
  invRm.m[10] = c + mc * u.z * u.z;
  tos->affn->postMult(rm);
  tos->invAffn->preMult(invRm);
}
void AffineStack :: scale(float sx, float sy, float sz)
{ // post-multiply top item by scaling
#define sEps 0.00001
  Affine4 scl;// make an identity
  Affine4 invScl;
  scl.m[0]  = sx; 
  scl.m[5]  = sy; 
  scl.m[10] = sz;// adjust it to a scaling matrix
  if(fabs(sx) < sEps || fabs(sy) < sEps || fabs(sz) < sEps)
    {
      cerr << "degenerate scaling transformation!\n";
    }
  invScl.m[0]  = 1/sx; invScl.m[5]  = 1/sy; invScl.m[10] = 1/sz;
  tos->affn->postMult(scl); //
  tos->invAffn->preMult(invScl);
}		
void AffineStack :: translate(Vector3 d)
{
  Affine4 tr; // make identity matrix
  Affine4 invTr;
  tr.m[12] = d.x; tr.m[13] = d.y;	tr.m[14] = d.z;
  invTr.m[12] = -d.x;	invTr.m[13] = -d.y; invTr.m[14] = -d.z;
  tos->affn->postMult(tr);
  tos->invAffn->preMult(invTr);
}

// Material methods
void Material :: setDefault(){
  textureType = 0; // for none
  numParams = 0;
  reflectivity = transparency = 0.0;
  speedOfLight = specularExponent = 1.0;
  specularFraction = 0.0;
  surfaceRoughness = 1.0;
  ambient.set(0.1f,0.1f,0.1f);
  diffuse.set(0.8f,0.8f,0.8f);
  specular.set(0,0,0);
  emissive.set(0,0,0);
}
void Material :: set(Material& m)
{
  textureType = m.textureType;
  numParams = m.numParams;
  for(int i = 0; i < numParams; i++) params[i] = m.params[i];
  transparency = m.transparency;
  speedOfLight = m.speedOfLight;
  reflectivity = m.reflectivity;
  specularExponent = m.specularExponent;
  specularFraction = m.specularFraction;
  surfaceRoughness = m.surfaceRoughness;
  ambient.set(m.ambient);
  diffuse.set(m.diffuse);
  specular.set(m.specular);
  emissive.set(m.emissive);
}
// Shape methods
void Shape :: tellMaterialsGL()
{
  float amb[4],diff[4],spec[4], emiss[4];
  float zero[] = {0,0,0,1};
  mtrl.ambient.build4tuple(amb); // fill the array
  mtrl.diffuse.build4tuple(diff);
  mtrl.specular.build4tuple(spec);
  mtrl.emissive.build4tuple(emiss);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_AMBIENT,amb);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_DIFFUSE,diff);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_SPECULAR,spec);
  glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_EMISSION,emiss);
  glMaterialf(GL_FRONT/*_AND_BACK*/,GL_SHININESS,mtrl.specularExponent);
}
//Mesh methods
Mesh :: Mesh(){
  numVerts = numFaces = numNorms = 0; 
  pt = NULL; norm  =  NULL; face = NULL;
  lastVertUsed = lastNormUsed = lastFaceUsed = -1;
}
void Mesh :: freeMesh()
{ // free up memory used by this mesh.
  delete [] pt; // release whole vertex list
  delete [] norm;
  for(int f = 0; f < numFaces; f++)
    delete[] face[f].vert; // delete the vert[] array of this face
  delete [] face;
}
int Mesh :: isEmpty() 
{
  return (numVerts == 0) || (numFaces == 0) || (numNorms == 0);
}
void Mesh :: makeEmpty() 
{ 
  numVerts = numFaces = numNorms = 0;
}
void Mesh :: drawOpenGL()
{ 
  if (debug) cout << "Start drawOpenGL()\n";
  tellMaterialsGL(); 	glPushMatrix();
  glMultMatrixf(transf.m); 
  drawMesh();	
  glPopMatrix();
  if (debug) cout << "End drawOpenGL()\n";
} 
Mesh :: Mesh(string fname){ // read this file to build mesh
  numVerts = numFaces = numNorms = 0; 
  pt = NULL; norm  =  NULL; face = NULL;
  lastVertUsed = lastNormUsed = lastFaceUsed = -1;
  readMesh(fname);
}
Vector3 Mesh :: newell4(int indx[])
{ /* return the normalized normal to face with vertices
     pt[indx[0]],...,pt[indx[3]]. i.e. indx[] contains the four indices 
     into the vertex list to be used in the Newell calculation */
  Vector3 m;
  for(int i = 0; i < 4 ; i++)
    {
      int next = (i== 3) ? 0 : i + 1; // which index is next?
      int f = indx[i], n = indx[next]; // names for the indices in the pair
      m.x += (pt[f].y - pt[n].y) * (pt[f].z + pt[n].z);
      m.y += (pt[f].z - pt[n].z) * (pt[f].x + pt[n].x);
      m.z += (pt[f].x - pt[n].x) * (pt[f].y + pt[n].y);
    }
  m.normalize();
  return m;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<< readMesh >>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: readMesh(string fname)
{
  fstream inStream;
  inStream.open(fname.c_str(), ios ::in); //open needs a c-like string
  if(inStream.fail() || inStream.eof()) 
    {
      cout << "can't open file or eof: " << fname << endl; 
      makeEmpty();return;
    }
  inStream >> numVerts >> numNorms >> numFaces;
  // make arrays for vertices, normals, and faces
  pt = new Point3[numVerts];        assert(pt != NULL);
  norm = new Vector3[numNorms];     assert(norm != NULL);
  face = new Face[numFaces];        assert(face != NULL);
  for(int i = 0; i < numVerts; i++) 	// read in the vertices
    inStream >> pt[i].x >> pt[i].y >> pt[i].z;
  for(int ii = 0; ii < numNorms; ii++) 	// read in the normals
    inStream >> norm[ii].x >> norm[ii].y >> norm[ii].z;
  for(int f = 0; f < numFaces; f++)   // read in face data
    {
      inStream >> face[f].nVerts;
      int n = face[f].nVerts;
      face[f].vert = new VertexID[n]; assert(face[f].vert != NULL);
      for(int k = 0; k < n; k++) 		// read vertex indices for this face
	inStream >> face[f].vert[k].vertIndex;
      for(int kk = 0; kk < n; kk++) 		// read normal indices for this face
	inStream >> face[f].vert[kk].normIndex;
    }
  inStream.close();
} // end of readMesh
//<<<<<<<<<<<<<<<<<<<<<< drawMesh >>>>>>>>>>>>>>>>>>>>
void Mesh :: drawMesh()
{ // draw each face of this mesh using OpenGL: draw each polygon.
  if (debug) cout << "Start drawMesh()\n";
  if(isEmpty()) return; // mesh is empty
  for(int f = 0; f < numFaces; f++)
    {
      int n = face[f].nVerts;
      glBegin(GL_POLYGON);
      for(int v = 0; v < n; v++)
	{
	  int in = face[f].vert[v].normIndex;
	  assert(in >= 0 && in < numNorms);
	  glNormal3f(norm[in].x, norm[in].y, norm[in].z);
	  int iv = face[f].vert[v].vertIndex; assert(iv >= 0 && iv < numVerts);
	  glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
	  if (debug) 
	    cout << "\tdrawMesh: Vertex3f" 
		 << pt[iv].x << ", " << pt[iv].y << ", " << pt[iv].z << endl;
	}
      glEnd();	
    }
  glFlush();
  if (debug) cout << "End drawMesh()\n";
}		
//<<<<<<<<<<<<<<<<<< hit() for Sphere >>>>>>>>>>>>>>>>>>>>>>>
bool Sphere:: hit(Ray &r, Intersection& inter)
{  // intersect the ray with the generic sphere
  Ray genRay;
  xfrmRay(genRay,invTransf,r);			
  double A, B, C, discrim, disc_root, t1, t2;			
  A = dot3D(genRay.dir, genRay.dir); assert(A > 0); // fails if ray is sick 
  B = dot3D(genRay.start, genRay.dir);
  C = dot3D(genRay.start, genRay.start) - 1.0;
  discrim = B * B - A * C;
  if(discrim < 0.0)return false;   // ray missed: numHits = 0 
  disc_root = sqrt(discrim);
			
  inter.numHits = 0;	// no hits yet	
  t1 = (-B - disc_root)/A;         // earlier hit 
  if(t1 > 0.00001) // a pos hit time
    {
      inter.hit[0].hitTime = t1;
      Point3 P(rayPos(genRay, t1));// position of hit spot
      inter.hit[0].hitPoint.set(P);
      inter.hit[0].hitObject = this;
      inter.hit[0].isEntering = 1;
      inter.hit[0].surface = 0;
      inter.hit[0].hitNormal.set(P.x, P.y, P.z); // vector is same as hit spot
      inter.numHits++; // got one hit so far
    }
  t2 = (-B + disc_root)/A; 		// second hit
  if(t2 > 0.00001) // got a hit (first or second...)
    {
      inter.hit[inter.numHits].hitTime = t2;
      Point3 P(rayPos(genRay, t2));  // position of hit spot
      inter.hit[inter.numHits].hitPoint.set(P);
      inter.hit[inter.numHits].isEntering = 0;
      inter.hit[inter.numHits].hitObject = this;
      inter.hit[inter.numHits].surface = 0;
      inter.hit[inter.numHits].hitNormal.set(P.x, P.y, P.z); // vector is same as hit spot
      inter.numHits++;			// got another
    }
  return inter.numHits > 0;			
}// end of hit() for Sphere
	
// Scene methods
//<<<<<<<< methods >>>>>>>>>>>
string Scene :: nexttoken(void) //########## nexttoken()
{
  char c;
  string token;
  int lastchar = 1;
  if (!f_in) {return(token); }
  if (f_in->eof()) {return(token);}
  while (f_in->get(c)) 
    {
      if (f_in->eof()) {
	return(token);
      }
      switch (c) {
      case '\n': nextline += 1;
      case ' ' :
      case '\t':
      case '\a':
      case '\b':
      case '\v':
      case '\f':
      case '\r': {
	if ( lastchar == 0 ) {return(token);}break; }
      case '{': {
	token = c; return(token); break;}
      case '}': {
	token = c;
	return(token);
	break; }
      case '!': {
	while ( c != '\n' && f_in->get(c)) {
	}
	nextline++; break;}
      default: {
	token = token + c;
	lastchar = 0;
			
	if ((f_in->peek() == '{') ||
	    (f_in->peek() == '}') ) {
	  if ( lastchar == 0 ) {
	    return(token);
	  } else {
	    f_in->get(c);
	    token = c;
	    return(token);
	  }
	}
	line = nextline;
      }
      }
    }
  return(" ");
}
//<<<<<<<<<<<<<< getFloat >>>>>>>>>>>>>>>>
float Scene :: getFloat() //############ getFloat()
{
  strstream tmp;
  float number;
  string str = nexttoken();  
  tmp << str;
  if(!(tmp >> number))
    {
      cerr << "Line " << line << ": error getting float" << endl;
      exit(-1);
    }
  else 
    {
      char t;
      if ( (tmp >> t ) ) 
	{
	  cerr << "Line " << line << ": bum chars in number" << endl;
	  exit(-1);    
	} 
    }
  return number;
}
//<<<<<<<<<<<<<<<<< isidentifier >>>>>>>>>>>>>>>>
bool Scene :: isidentifier(string keyword) { //######## isidentifier
  string temp = keyword;
  if (!isalpha(temp[0])) return(false);
  for (int count = 1; count < temp.length(); count++) {
    if ((!isalnum(temp[count]))&& (temp[count]!='.')) return(false);
  }
  return(true);
}
//<<<<<<<<<<<<<<<< cleanUp >>>>>>>>>>>>>>>>
void Scene :: cleanUp() //######### cleanUp
{ // release stuff after parsing file
  affStk.releaseAffines(); 		//delete affine stack
  def_stack->release();
  delete def_stack; // release the DefUnitStack memory
}
//<<<<<<<<<<<<<<<<< freeScene >>>>>>>>>>>>>>
void Scene :: freeScene() 
{ // release the object and light lists
  GeomObj *p = obj;
  while(p)
    {
      GeomObj* q = p;
      p = p->next;
      delete q;
    }
  Light * q = light;
  while(q)
    {
      Light* r = q;
      q = q->next;
      delete r;
    }
}
//<<<<<<<<<<<<<<<<< whichToken >>>>>>>>>>>>>>>
mTokenType Scene :: whichtoken(string keyword) 
{
  string temp = keyword;
  if ( temp == "light" )				 return LIGHT;
  if ( temp == "rotate" )           return ROTATE;
  if ( temp == "translate" )        return TRANSLATE;
  if ( temp == "scale")             return (SCALE);	
  if ( temp == "push")        		 return (PUSH);
  if ( temp == "pop")					 return (POP);
  if ( temp == "identityAffine")    return (IDENTITYAFFINE);
  if ( temp == "cube")              return (CUBE);
  if ( temp == "sphere")            return (SPHERE);
  if ( temp == "torus")             return (TORUS);
  if ( temp == "plane")             return (PLANE);
  if ( temp == "square")            return (SQUARE);
  if ( temp == "cylinder")          return (CYLINDER);
  if ( temp == "taperedCylinder")   return (TAPEREDCYLINDER);
  if ( temp == "cone")              return (CONE);
  if ( temp == "tetrahedron")       return (TETRAHEDRON);
  if ( temp == "octahedron")        return (OCTAHEDRON);
  if ( temp == "dodecahedron")      return (DODECAHEDRON);
  if ( temp == "icosahedron")       return (ICOSAHEDRON);
  if ( temp == "buckyball")         return (BUCKYBALL);
  if ( temp == "diamond")           return (DIAMOND);
  if ( temp == "teapot")				 return (TEAPOT);
  if ( temp == "union")             return (UNION);
  if ( temp == "intersection")      return (INTERSECTION);
  if ( temp == "difference")        return (DIFFERENCEa);
  if ( temp == "mesh")              return (MESH);
  if ( temp == "makePixmap")        return (MAKEPIXMAP);
  if ( temp == "defaultMaterials")  return (DEFAULTMATERIALS);
  if ( temp == "ambient")           return (AMBIENT);
  if ( temp == "diffuse")           return (DIFFUSE);
  if ( temp == "specular")          return (SPECULAR);
  if ( temp == "specularFraction")  return (SPECULARFRACTION);
  if ( temp == "surfaceRoughness")  return (SURFACEROUGHNESS);
  if ( temp == "emissive")          return (EMISSIVE);
  if ( temp == "specularExponent")  return (SPECULAREXPONENT);
  if ( temp == "speedOfLight")      return (SPEEDOFLIGHT);
  if ( temp == "transparency")      return (TRANSPARENCY);
  if ( temp == "reflectivity")      return (REFLECTIVITY);
  if ( temp == "parameters")        return (PARAMETERS);
  if ( temp == "texture")				 return (TEXTURE);
  if ( temp == "globalAmbient")	    return (GLOBALAMBIENT);
  if ( temp == "minReflectivity")	 return (MINREFLECTIVITY);
  if ( temp == "minTransparency")	 return (MINTRANSPARENCY);
  if ( temp == "maxRecursionDepth") return (MAXRECURSIONDEPTH);
  if ( temp == "background")        return (BACKGROUND);
  if ( temp == "{")                 return (LFTCURLY);
  if ( temp == "}")                 return (RGHTCURLY);
  if ( temp == "def")               return (DEF);
  if ( temp == "use")               return (USE);
  if ( temp == " " )                return (T_NULL);
  if ( isidentifier(temp) )         return (IDENT);
  if(info) cout << temp << ":" << temp.length() << endl;
  return(UNKNOWN);
} // end of whichtoken

//<<<<<<<<<<  drawSceneOpenGL >>>>>>>>>>>>>>>>.
void Scene :: drawSceneOpenGL()
{ //draw each object on object list
  if (debug) cout << "Start drawSceneOpenGL()\n";
  for(GeomObj* p = obj; p ; p = p->next) {
    if (debug) cout << "\tdrawSceneOpenGL: Drawing " << p << endl;
    p->drawOpenGL(); //draw it
  }
  if (debug) cout << "end drawSceneOpenGL()\n";
}
//<<<<<<<<<<<<<<< Scene :: read >>>>>>>>>>>>>>>>
bool Scene:: read(string fname)// return true if ok; else false
{
  file_in = new ifstream(fname.c_str());
  if(!(*file_in))
    { 
      cout << "I can't find or open file: " << fname << endl; 
      return false;
    }
  f_in = new strstream();
  line = nextline = 1;
  def_stack = new DefUnitStack();
  char ch;
  freeScene(); //delete any previous scene	
  // initialize all for reading:
  obj = tail = NULL;
  light = NULL;
  affStk.tos = new AffineNode;
  affStk.tos->next = NULL;
	
  while (file_in->get(ch)) {*f_in << ch;} // read whole file
  while(1) //read file, collecting objects, until EOF or an error
    {
      GeomObj * shp = getObject(); // get the next shape
      if(!shp) break; // no object: either error or EOF
      shp->next = NULL; // to be safe
      if(obj == NULL){ obj = tail = shp;} // empty list so far
      else{tail->next = shp; tail = shp;} // add new object to queue
    }	
  file_in->close();
  cleanUp(); // delete temp lists, etc.
  return true;
} // end of read()

//<<<<<<<<<<<<<< Scene :: getObject >>>>>>>>>>>>>>>	
GeomObj* Scene :: getObject()
{ //reads tokens from stream f_in (a data member of Scene),
  // building lights, getting materials, doing transformations,
  // until it finds a new object
  // returns NULL if any error occurs, or end of file
  string s;			
  GeomObj * newShape;
  mTokenType typ;
  while ((typ = (whichtoken( s = nexttoken() ))) != T_NULL) 
    {
      if(typ == UNION || typ == INTERSECTION || typ == DIFFERENCEa)
	{
	  switch(typ)
	    {
	    case UNION:				newShape = new UnionBool();	break;
	    case INTERSECTION:	newShape = new IntersectionBool();	break;
	    case DIFFERENCEa:		newShape = new DifferenceBool();break;
	    } // end of little switch
	  GeomObj* p = newShape;
	  p = getObject(); // get left child
	  if(!p) return NULL; // Error! should always get an object
	  ((Boolean*)newShape)->left  = p; // hook it up
	  p = getObject();// get right child
	  if(!p) return NULL;
	  ((Boolean*)newShape)->right = p; // hook it up
	  return newShape;
	}// end of if(typ == UNION etc....
      switch(typ) 
	{
	case LIGHT: {
	  Point3 p;
	  Color3 c;
	  p.x = getFloat(); p.y = getFloat();	p.z = getFloat();
	  c.red = getFloat(); c.green = getFloat();	c.blue = getFloat();
	  Light *l = new Light;
	  l->setPosition(p);
	  l->setColor(c);
	  l->next = light; //put it on the list
	  light = l; break;}
	case ROTATE: {
	  float angle;
	  Vector3 u;
	  angle = getFloat(); u.x = getFloat(); 
	  u.y = getFloat(); u.z = getFloat();
	  affStk.rotate(angle,u);break;}
	case TRANSLATE: {
	  Vector3 d;
	  d.x = getFloat(); d.y = getFloat(); d.z = getFloat();
	  affStk.translate(d);break;}
	case SCALE: {
	  float sx, sy, sz;
	  sx = getFloat(); sy = getFloat(); sz = getFloat();
	  affStk.scale(sx, sy, sz);break;}
	case PUSH: affStk.dup(); break; 
	case POP:  affStk.popAndDrop(); break;
	case IDENTITYAFFINE: affStk.setIdentity();break;		
	case AMBIENT: {
	  float dr, dg, db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.ambient.set(dr,dg,db); break;}
	case DIFFUSE: {
	  float dr,dg,db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.diffuse.set(dr,dg,db); break;}
	case SPECULAR:{ 
	  float dr,dg,db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.specular.set(dr,dg,db); break;}
	case EMISSIVE: {
	  float dr,dg,db;
	  dr = getFloat(); dg = getFloat(); db = getFloat();
	  currMtrl.emissive.set(dr,dg,db); break;}
	case PARAMETERS: { // get a list of numParams parameters
	  currMtrl.numParams = (int)getFloat();
	  for(int i = 0; i < currMtrl.numParams; i++)
	    currMtrl.params[i] = getFloat();
	  break;}			
	case SPECULARFRACTION: currMtrl.specularFraction = getFloat(); break;
	case SURFACEROUGHNESS: currMtrl.surfaceRoughness = getFloat(); break;
	case TEXTURE: { // get type, 0 for none
	  currMtrl.textureType = (int) getFloat();}	
	break;
	case DEFAULTMATERIALS: 	currMtrl.setDefault();break;
	case SPEEDOFLIGHT: currMtrl.speedOfLight = getFloat(); break;
	case SPECULAREXPONENT: currMtrl.specularExponent = getFloat(); break;
	case TRANSPARENCY:currMtrl.transparency = getFloat(); break;
	case REFLECTIVITY: currMtrl.reflectivity = getFloat(); break;
	case GLOBALAMBIENT:
	  ambient.red = getFloat(); ambient.green = getFloat();
	  ambient.blue = getFloat(); break;
	case BACKGROUND: 
	  background.red = getFloat();
	  background.green = getFloat(); 
	  background.blue = getFloat();break;
	case MINREFLECTIVITY: minReflectivity = getFloat(); break;
	case MINTRANSPARENCY:minTransparency = getFloat(); break;
	case MAXRECURSIONDEPTH: maxRecursionDepth = (int) getFloat(); break;
	case MAKEPIXMAP: {	// get BMP file name for a pixmap			
	  /* to be implemented, along the lines:
	     int which = getFloat();// index of this pixmap in pixmap array
	     if(which < 0 || which > 7){cout << "\nbad index of RGBpixmap!\n";}
	     string fname = nexttoken(); // get file name for mesh	  
	     cout << "I got fname = " << fname << endl;
	     if(!pixmap[which].readBMPFile(fname))
	     {// read BMP file into this pixmap
	     cout << " \ncan't read that RGBpixmap file!\n";
	     return NULL;  }  */
	  break;}// end of case: MAKEPIXMAP
	case T_NULL: break; // The null token represents end-of-file 
	case DEF: {
	  string name, temp, lb, rb;
	  int l = line;
	  string inp;
	  name = nexttoken();
	  if ( whichtoken(name) != IDENT ) {
	    cout << "Error:  Identifier expected." << endl;
	    return NULL;
	  }
	  if ( def_stack->search(name) ) {
	    cout << line << ": " << name;
	    cout << ": attempt to redefine. " << endl;
	    return NULL;
	  }
	  lb = nexttoken();
	  if ( whichtoken(lb) != LFTCURLY ) {
	    cout << "Error: { expected." << endl;
	    return NULL;
	  }
	  while ( whichtoken( temp = nexttoken()) != RGHTCURLY ) {
	    if (info) cout << temp << endl;
	    inp = inp + temp + " ";
	    if (!f_in) {
	      cout << "Error: end of file detected." << endl;
	      return NULL;
	    }
	  }
	  // Push the contents of the string on the stack.
	  def_stack->push(name, inp);
	  break;} // end of case: DEF
	case USE: {
	  string name;
	  name = nexttoken();
	  if ( whichtoken(name) != IDENT ) {
	    cout << line << ": " << name;
	    cout << ": identifier expected.";
	    return NULL;
	  }
	  if (! def_stack->search(name) ) {
	    cout << line << ": " << name;
	    cout << ": not defined.";
	    return NULL;
	  }
	  if(info) cout << def_stack->contents(name) << endl;
	  strstream *temp_fin = new strstream;
	  *temp_fin << def_stack->contents(name) << " ";
	  *temp_fin << f_in->rdbuf();
	  delete (f_in);
	  f_in = temp_fin;
	  break; } // end of case: USE
	default:  { // inner switch for Shapes
	  switch(typ) 
	    {
				//float param;
	    case CUBE:			newShape = new Cube;break;
	    case SPHERE:		newShape = new Sphere;break;
	    case TETRAHEDRON:	newShape = new Mesh("tetra.3vn");break;
	    case TORUS:		newShape = new Torus;break;
	    case PLANE:		newShape = new Plane;break;
	    case SQUARE:		newShape = new Square;break;
	    case TAPEREDCYLINDER:	newShape = new TaperedCylinder; 
	      ((TaperedCylinder*)newShape)->smallRadius = getFloat(); break;
	    case CONE:			newShape = new TaperedCylinder; 
	      ((TaperedCylinder*)newShape)->smallRadius = 0; break;
	    case CYLINDER:		newShape = new TaperedCylinder; 
	      ((TaperedCylinder*)newShape)->smallRadius = 1; break;
	    case OCTAHEDRON:		newShape = new Mesh("octa.3vn");break;
	    case DODECAHEDRON:	newShape = new Mesh("dodeca.3vn"); break;
	    case ICOSAHEDRON:	newShape = new Mesh("icosa.3vn"); break;
	    case BUCKYBALL:		newShape = new Mesh("bucky.3vn");	break;
	    case DIAMOND:		newShape = new Mesh("diamond.3vn"); break;
	    case TEAPOT:		newShape = new Teapot; break;
	    case MESH: {// get a filename (with extension) for this mesh		
	      string fname = nexttoken(); // get file name for mesh
	      newShape = new Mesh(fname); break;
	    }// end of case: MESH
	    default: {
	      cerr << "Line " << nextline << ": unknown keyword " << s << endl;
	      return NULL;
	    }	
	    } // end of inner switch
	  // common things to do to all Shape’s
	  ((Shape*)newShape)->mtrl.set(currMtrl);
	  // load transform and its inverse
	  ((Shape*)newShape)->transf.set(*(affStk.tos->affn));
	  ((Shape*)newShape)->invTransf.set(*(affStk.tos->invAffn));
	  return newShape;
	}// end of default: block			
	} // end of outer switch
    } // end of while
  return NULL;
} // end of getObject

// DefUnitStack methods
void DefUnitStack :: push(string n, string s) { 
  D4S *temp_d4s = new D4S;
  temp_d4s->current = new DefUnit(n, s);
  temp_d4s->next = stack;
  stack = temp_d4s;
}
void DefUnitStack :: print() {
  D4S *temp = stack;
  string t;
  while (temp) {
    if(info) cout << temp->current->name << ":" ;
    if(info) cout << temp->current->stuff << endl;
    temp = temp->next;
  }
}
int DefUnitStack :: search(string s) {
  D4S *temp = stack;
  while (temp) {
    if ( temp->current->name == s ) {
      return(1);
    }
    temp = temp->next;
  }
  return(0);
}
string DefUnitStack :: contents(string s) {
  D4S *temp = stack;
  while (temp) {
    if (temp->current->name == s ) {
      return(temp->current->stuff);
    }
    temp = temp->next;
  }
  return(NULL);
}
void DefUnitStack :: release()
{
  while(stack)
    {
      D4S* tmp = stack; // grab it
      stack = stack->next; // advance p
      delete tmp->current; // release 2 strings
      delete tmp; // release node
    }
  stack = NULL;
}
//<<<<<<<<<<<<<<<<<<<<<<<< shade >>>>>>>>>>>>>>>>>>>>>>>>>>>
Color3 Scene :: shade(Ray& ray) // return the color of this ray
{
  Color3 color; //for ambient and diffuse with texturing
  Intersection inter, bestInter; // construct a new intersection record	
  bestInter.numHits = 0; // no hits at the start
	
  for(GeomObj* pOb = obj; pOb != NULL; pOb = pOb->next)  // test each object in the scene
    {
      if(!pOb->hit(ray, inter)) // have a hit at a positive hit time?
	continue; // nope: a miss
      if(bestInter.numHits == 0 || inter.hit[0].hitTime < bestInter.hit[0].hitTime) 
	bestInter.set(inter);  // copy inter into best
    } // end of replacement for getFirstHit()	
  if(bestInter.numHits == 0)
    return background; // no objects were hit
  HitInfo h;
  h.set(bestInter.hit[0]); //copy first hit record of bestInter
  Point3 wHitPoint(ray.start.x + ray.dir.x * h.hitTime, 
		   ray.start.y + ray.dir.y * h.hitTime, 
		   ray.start.z + ray.dir.z * h.hitTime); 
  Shape* myObj = (Shape*)h.hitObject; //point to hit object
  color.set(myObj->mtrl.emissive); //start with emissive part;
  return color;
} // end of Scene :: shade
