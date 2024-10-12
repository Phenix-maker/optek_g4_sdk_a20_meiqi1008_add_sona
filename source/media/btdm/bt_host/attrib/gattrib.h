#ifndef __GATTRIB_H
#define __GATTRIB_H

#define GATTRIB_ALL_EVENTS 0xFF
#define GATTRIB_ALL_REQS 0xFE
#define GATTRIB_ALL_HANDLES 0x0000

struct _GAttrib;
typedef struct _GAttrib GAttrib;

struct gatt_prim {
	uint16_t uuid_16;
	struct att_range range;
};

typedef void (*GAttribResultFunc) (guint8 status, const guint8 *pdu,
					guint16 len, gpointer user_data);
typedef void (*GAttribDisconnectFunc)(gpointer user_data);
typedef void (*GAttribDebugFunc)(const char *str, gpointer user_data);
typedef void (*GAttribNotifyFunc)(const guint8 *pdu, guint16 len,
							gpointer user_data);

//added by Optek
typedef void (*GDestroyNotify)(gpointer user_data);
typedef void (*GIOChannel)(void);


//GAttrib *g_attrib_new(GIOChannel *io);
GAttrib *g_attrib_ref(GAttrib *attrib);
void g_attrib_unref(GAttrib *attrib);

GIOChannel *g_attrib_get_channel(GAttrib *attrib);


gboolean g_attrib_set_destroy_function(GAttrib *attrib,
		GDestroyNotify destroy, gpointer user_data);

guint g_attrib_send(GAttrib *attrib, guint id, const guint8 *pdu, guint16 len,
			GAttribResultFunc func, gpointer user_data,
			GDestroyNotify notify);

gboolean g_attrib_cancel(GAttrib *attrib, guint id);
gboolean g_attrib_cancel_all(GAttrib *attrib);

gboolean g_attrib_set_debug(GAttrib *attrib,
		GAttribDebugFunc func, gpointer user_data);

guint g_attrib_register(GAttrib *attrib, guint8 opcode, guint16 handle,
				GAttribNotifyFunc func, gpointer user_data,
				GDestroyNotify notify);

gboolean g_attrib_is_encrypted(GAttrib *attrib);

uint8_t *g_attrib_get_buffer(GAttrib *attrib, size_t *len);
gboolean g_attrib_set_mtu(GAttrib *attrib, int mtu);

gboolean g_attrib_unregister(GAttrib *attrib, guint id);
gboolean g_attrib_unregister_all(GAttrib *attrib);

void g_attrib_primary_service(GAttrib *attrib, struct gatt_prim *service); 

#endif
