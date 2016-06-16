//---------------------------------------------------------------------------

#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

//---------------------------------------------------------------------------

#include <graphics/image/ImageData.h>
#include <core/container/Map.h>
#include <core/addition/HandleSingleton.h>
#include <core/String.h>
#include <core/Exception.h>

//---------------------------------------------------------------------------

namespace Rapture
{
	class ImageDecoder
	{
		friend class ImageIO;
		
	public:
		virtual ~ImageDecoder() {}

	protected:
		virtual void decode(ImageData * output, const string & type, const ByteData * raw) = 0;
	};

	class ImageEncoder
	{
		friend class ImageIO;
		
	public:
		virtual ~ImageEncoder() {}

	protected:
		virtual void encode(OwnedByteData * output, const string & type, const ImageData * image) = 0;
	};

	class ImageConverter : public ImageDecoder, public ImageEncoder
	{
	public:
		virtual ~ImageConverter() {}
	};
	
	class ImageConversionException : public Exception
	{
	public:
		using Exception::Exception;
		virtual ~ImageConversionException() {}
	};

	class ImageIO : public Singleton<ImageIO>
	{
	public:
		static void read(ImageData * output, const string & type, const ByteData * raw)
		{
			auto & decoder = instance().decoders[type];

			if(decoder == nullptr)
				throw ImageConversionException("Can't read image data of type ", type);

			decoder->decode(output, type, raw);
		}

		static void write(OwnedByteData * output, const string & type, const ImageData * image)
		{
			auto & encoder = instance().encoders[type];

			if(encoder == nullptr)
				throw ImageConversionException("Can't write image data of type ", type);

			encoder->encode(output, type, image);
		}

		static void api(graphics) load(ImageData * output, const path & filepath);
		static void api(graphics) save(const path & filepath, const ImageData * image);

		static ImageDecoder * getDecoder(const string & type)
		{
			return instance().decoders[type];
		}

		static ImageEncoder * getEncoder(const string & type)
		{
			return instance().encoders[type];
		}

		template<class Decoder, useif <
			based_on<Decoder, Singleton<Decoder>>::value,
			based_on<Decoder, ImageDecoder>::value
			> endif
		>
		static void setDecoder(const string & type, Type<Decoder> = {})
		{
			instance().decoders[type] = &Decoder::instance();
		}

		template<class Encoder, useif <
			based_on<Encoder, Singleton<Encoder>>::value,
			based_on<Encoder, ImageEncoder>::value
			> endif
		>
		static void setEncoder(const string & type, Type<Encoder> = {})
		{
			instance().encoders[type] = &Encoder::instance();
		}

		template<class Converter, useif <
			based_on<Converter, Singleton<Converter>>::value,
			based_on<Converter, ImageDecoder>::value,
			based_on<Converter, ImageEncoder>::value
			> endif
		>
		static void setConverter(const string & type, Type<Converter> = {})
		{
			auto & inst = instance();
			inst.decoders[type] = &Converter::instance();
			inst.encoders[type] = &Converter::instance();
		}
		
	protected:
		map<string, ImageDecoder *> decoders;
		map<string, ImageEncoder *> encoders;
	};

	template struct api(graphics) Singleton<ImageIO>;
}

//---------------------------------------------------------------------------
#endif