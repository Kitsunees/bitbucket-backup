/*
 *  display_ent.h
 *  Jumpcore
 *
 *  Created by Andi McClure on 10/28/13.
 *  Copyright 2013 Run Hello. All rights reserved.
 *
 */

#ifndef _DISPLAY_ENT_H
#define _DISPLAY_ENT_H

#include "kludge.h"
#include "display.h"
#include "displaycodes.h"
#include "ent.h"

typedef hash_map<display_code, pile *> quad_db;
typedef quad_db::iterator quad_db_iter;

#define RECT(from, to) { cpv((from).x,(from).y), cpv((from).x, (to).y), cpv( (to).x, (to).y), cpv( (to).x,(from).y), }

// Infrastructure

struct drawing { // Should track framebuffer
	float w, h;
	drawing();
	~drawing();
	quad_db data; // TODO: Preserve order?
	pile &get_pile(display_code code = D_C, elementoid *_elementDrawer = NULL, bool _useThree = false, bool _useTexture = true, bool _useColor = false, GLint _texture = 0, unsigned int _baseColor = WHITE);
	quadpile &get_quadpile(display_code code = D_C, bool _useTexture = true, bool _useColor = false, GLint _texture = 0, unsigned int _baseColor = WHITE);
	quadpile3 &get_quadpile3(display_code code = D_C3, bool _useTexture = true, bool _useColor = false, GLint _texture = 0, unsigned int _baseColor = WHITE);

	void dupe(drawing *target);
	void size(texture_slice *target);
	void execute(); // Actually draw everything in data
};

struct texture_source {
	texture_slice *outTexture;
	bool ownTexture;
	display_code code;
	texture_source(texture_slice *s, bool own = false);
	texture_source(const char *name);
	virtual ~texture_source();
	void texture_load(texture_slice *s, bool own = false);
	void texture_load(const char *name);
};

// Ents

struct lockout : public ent {
	lockout() : ent() {}
	void display(drawing *);
};

struct display_ent : public ent {
	display_ent() : ent() {}
	void display(drawing *);	
};

struct splatter : public ent {
	texture_source *src;
	bool ownSrc;
	stateoid *stateSetter; // Always owned
	splatter(texture_source *_src, bool _ownSrc = false, stateoid *_stateSetter = NULL) : ent(), src(_src), ownSrc(_ownSrc), stateSetter(_stateSetter) {}
	~splatter() { if (ownSrc) delete src; delete stateSetter; }
	void display(drawing *);
};

struct fixed_splatter : public splatter {
	cpVect size, offset;
	fixed_splatter(texture_source *_src, bool _ownSrc = false, stateoid *_stateSetter = NULL, cpVect _size = cpv(1,1), cpVect _offset = cpvzero) : splatter(_src, _ownSrc, _stateSetter), size(_size), offset(_offset) {}
	void display(drawing *);
};

struct boxer : public ent {
	cpVect size, offset;
	boxer(cpVect _size = cpv(1,1), cpVect _offset = cpvzero) : ent(), size(_size), offset(_offset) {}
	void display(drawing *);
};

#endif