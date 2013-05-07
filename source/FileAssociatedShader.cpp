
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include <glow/FileAssociatedShader.h>


using namespace std;

namespace glow
{
//
//FileAssociatedShader::t_shadersByFilePath FileAssociatedShader::shadersByFilePath;
//
//FileAssociatedShader::FileAssociatedShader(
//    const GLenum type
//,   const std::string & filePath)
//
//:   Shader(type)
//,   m_filePath(filePath)
//{
//	ifstream file(m_filePath);
//	if(!file.good())
//	{
//        cerr << "\"" << m_filePath << "\" does not exist: shader is without source and associated file." << endl;
//        return;
//	}
//
//    registerShader(m_filePath, this);
//
//    if(setSourceFromFile(false))
//        update();
//}
//
//FileAssociatedShader::~FileAssociatedShader()
//{
//	unregisterShader(m_filePath, this);
//}
//
//const bool FileAssociatedShader::setSourceFromFile(const bool update)
//{
//	// http://insanecoding.blogspot.de/2011/11/how-to-read-in-file-in-c.html
//
//	ifstream stream(m_filePath, ios::in);
//	if(!stream)
//	{
//        cerr << "Read from \"" << m_filePath << "\" failed." << endl;
//        return NULL;
//    }
//
//	ostringstream source;
//    source << stream.rdbuf();
//    stream.close();
//
//	return setSource(source.str(), update);
//}
//
//void FileAssociatedShader::registerShader(
//	const std::string & filePath
//,	FileAssociatedShader * shader)
//{
//	t_shadersByFilePath::iterator f(shadersByFilePath.find(filePath));
//
//    if(shadersByFilePath.end() != f)
//    {
//		t_shaders * shaders(f->second);
//
//		assert(shaders);
//		assert(!shaders->empty());
//
//		assert(shaders->end() == shaders->find(shader));
//
//		shaders->insert(shader);
//    }
//    else
//    {
//		t_shaders * shaders(new t_shaders);
//		shaders->insert(shader);
//
//		shadersByFilePath[filePath] = shaders;
//    }
//}
//
//void FileAssociatedShader::unregisterShader(
//	const std::string & filePath
//,	FileAssociatedShader * shader)
//{
//	t_shadersByFilePath::iterator f(shadersByFilePath.find(filePath));
//
//	assert(shadersByFilePath.end() != f);
//
//	t_shaders * shaders(f->second);
//
//	assert(shaders->end() != shaders->find(shader));
//
//	shaders->erase(shader);
//
//	if(shaders->empty())
//	{
//		delete f->second;
//		shadersByFilePath.erase(filePath);
//	}
//}
//
//void FileAssociatedShader::reloadAll()
//{
//	t_shadersByFilePath::const_iterator f(shadersByFilePath.cbegin());
//	const t_shadersByFilePath::const_iterator fEnd(shadersByFilePath.cend());
//
//	for(; f != fEnd; ++f)
//	{
//		t_shaders::const_iterator s(f->second->cbegin());
//		const t_shaders::const_iterator sEnd(f->second->cend());
//
//		for(; s != sEnd; ++s)
//			(*s)->reload();
//	}
//}
//
//void FileAssociatedShader::reload()
//{
//	ifstream file(m_filePath);
//	if(!file.good())
//	{
//        cerr << "\"" << m_filePath << "\" does not exist: shader will not reload from associated file." << endl;
//        return;
//	}
//
//    if(isCompiled())
//    {
//        // if current version works, use its source code as
//        // backup if new changes lead to uncompilable shader.
//
//		const std::string backup(source());
//        if(setSourceFromFile(false))
//        {
//            update();
//            return;
//        }
//
//		setSource(backup, true);
//		assert(isCompiled());
//	}
//    else
//		setSourceFromFile();
//}

} // namespace glow
