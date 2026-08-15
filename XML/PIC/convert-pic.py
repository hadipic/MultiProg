#!/usr/bin/env python3
# optimize-pic-fuses.py
# بهینه‌سازی و فشرده‌سازی فایل‌های pic-fuses-*.js

import os
import json
import re
from pathlib import Path

def optimize_config(config):
    """بهینه‌سازی یک config word"""
    # حذف فیلدهای خالی
    if not config.get('bits'):
        config.pop('bits', None)
    if not config.get('navigation'):
        config.pop('navigation', None)
    return config

def compress_navigation(navigation):
    """فشرده‌سازی navigation با حذف members تکراری"""
    if not navigation:
        return []
    
    # حذف نویگیشن‌های تکراری (با mask یکسان)
    seen_masks = set()
    unique_nav = []
    for nav in navigation:
        mask = nav.get('mask', 0)
        if mask in seen_masks:
            continue
        seen_masks.add(mask)
        
        # فشرده‌سازی members: حذف موارد تکراری
        members = nav.get('members', [])
        if members:
            # اگر همه‌ی members مقدار یکسانی دارند (مثلاً فقط On/Off)
            # می‌توانیم آن را به یک بیت ساده تبدیل کنیم
            if len(members) == 2:
                val1 = members[0].get('value')
                val2 = members[1].get('value')
                text1 = members[0].get('text')
                text2 = members[1].get('text')
                # اگر فقط On/Off است، نیازی به navigation کامل نیست
                if (text1 in ['Off', 'On', 'Disabled', 'Enabled'] and 
                    text2 in ['Off', 'On', 'Disabled', 'Enabled']):
                    # به‌عنوان بیت ساده ذخیره می‌شود (در مرحله‌ی بعد)
                    continue
        
        unique_nav.append(nav)
    return unique_nav

def optimize_device_data(device_data):
    """بهینه‌سازی داده‌های یک دستگاه"""
    optimized = []
    for config in device_data:
        # حذف فیلدهای خالی
        config = optimize_config(config)
        if config.get('navigation'):
            config['navigation'] = compress_navigation(config['navigation'])
        # اگر بعد از بهینه‌سازی کاملاً خالی شد، حذف کن
        if not config.get('bits') and not config.get('navigation'):
            # فقط default و name را نگه دار
            config = {'name': config['name'], 'default': config['default']}
        optimized.append(config)
    return optimized

def process_file(input_path, output_path=None):
    """پردازش یک فایل و ذخیره‌ی نسخه‌ی بهینه‌شده"""
    if output_path is None:
        output_path = input_path.replace('.js', '.opt.js')
    
    with open(input_path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    # استخراج JSON از محتوای فایل
    # الگوی: export const picFuseDB_18F = {...};
    match = re.search(r'export const picFuseDB_\w+\s*=\s*({.*?});', content, re.DOTALL)
    if not match:
        print(f"⚠️ فرمت فایل {input_path} قابل تشخیص نیست.")
        return
    
    json_str = match.group(1)
    try:
        data = json.loads(json_str)
    except json.JSONDecodeError as e:
        print(f"⚠️ خطا در parse JSON فایل {input_path}: {e}")
        return
    
    # بهینه‌سازی داده‌ها
    for device, configs in data.items():
        data[device] = optimize_device_data(configs)
    
    # تولید خروجی فشرده
    json_optimized = json.dumps(data, ensure_ascii=False, separators=(',', ':'))
    
    # استخراج نام متغیر
    var_match = re.search(r'export const (picFuseDB_\w+)', content)
    var_name = var_match.group(1) if var_match else 'picFuseDB'
    
    # تولید فایل جدید
    js_content = f"""// {os.path.basename(input_path)} (بهینه‌شده)
export const {var_name} = {json_optimized};

export const {var_name.replace('picFuseDB_', 'picFuseDBProxy_')} = new Proxy({var_name}, {{
  get(target, prop) {{
    if (prop in target) return target[prop];
    return [];
  }}
}});
"""
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(js_content)
    
    # گزارش حجم
    old_size = os.path.getsize(input_path) / 1024
    new_size = os.path.getsize(output_path) / 1024
    reduction = ((old_size - new_size) / old_size) * 100
    print(f"✅ {os.path.basename(input_path)}: {old_size:.1f}KB -> {new_size:.1f}KB (کاهش {reduction:.1f}%)")

def main():
    # پردازش همه‌ی فایل‌های pic-fuses-*.js در پوشه‌ی فعلی
    files = [f for f in os.listdir('.') if f.startswith('pic-fuses-') and f.endswith('.js')]
    if not files:
        print("❌ هیچ فایل pic-fuses-*.js در این پوشه یافت نشد.")
        return
    
    # ایجاد پوشه‌ی خروجی
    os.makedirs('optimized', exist_ok=True)
    
    for filename in files:
        input_path = filename
        output_path = os.path.join('optimized', filename)
        process_file(input_path, output_path)
    
    print(f"\n📁 فایل‌های بهینه‌شده در پوشه‌ی 'optimized' قرار دارند.")

if __name__ == "__main__":
    main()
